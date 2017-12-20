/* 
 * Author:  Larisa Markeeva, Innopolis University and Erik Billing, University of Skovde
 *
 * Copyright (C) 2016 DREAM Consortium
 * FP7 Project 611391 co-funded by the European Commission
 *
 * Author:  Erik Billing, University of Skovde 
 * Email:   erik.billing@his.se 
 * Website: www.dream2020.eu 
 * 
 * This file is part of DREAM.
 * 
 * DREAM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * DREAM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with DREAM.  If not, see <http://www.gnu.org/licenses/>.
 */




/*
 * Audit Trail
 * -----------
 * 01/04/15  First version validated: David Vernon for Larisa Markeev
 */ 

#include "videoSource.h"

VideoSourceThread::VideoSourceThread(Port *events, BufferedPort<ImageOf<PixelRgb>> *imageOut, BufferedPort<VectorOf<long>> *frameOut, int *width, int *height, bool *heartbeatPlay, double *fps, bool *repeat): 
RateThread(*fps == 0 ? 0 : 1000.0 / *fps), videoFile("")
{
   debug                    = false;

	this->events             = events;
   imagePortOut             = imageOut;
	frameNumberOut           = frameOut;
	frameNumber              = 0;
   widthValue               = width;
   heightValue              = height;
	heartbeatPlayback        = heartbeatPlay;
	repeatPlayback           = repeat;
   
   if (debug) {
      cout << "VideoSourceThread: width            " << *widthValue << endl;
      cout << "VideoSourceThread: height           " << *heightValue << endl;
   }
}

bool VideoSourceThread::start(bool passivePlayback) {
	if (passivePlayback) {
		return threadInit();
	} else {
		return RateThread::start();
	}
}

bool VideoSourceThread::threadInit() 
{

   /* open the image file and create an image */
   
   if (debug) {
      cout << "VideoSourceThread::threadInit: video file name  " << videoFile << endl;
   }

	if (videoFile.empty()) {
		cout << "VideoSourceThread::threadInit: No video file specified." << endl;
		return false;
	} else {
		this->video.open(videoFile);
		if (video.isOpened()) {
			cout << "VideoSourceThread::threadInit: video file open." << endl;
			if (getRate() == 0) {
				setRate(1000/video.get(CV_CAP_PROP_FPS));
				cout << "VideoSourceThread::initThread: Using frame rate " << video.get(CV_CAP_PROP_FPS) << "." << endl;
			}
			return true;
		} else {
			cout << "VideoSourceThread::threadInit: unable to open video file" << endl;
			return false;
		}
	}
}

void VideoSourceThread::setSourceFile(string sourceFile) {
	videoFile = sourceFile;
	cout << "VideoSourceThread:sourceFile=" << sourceFile << endl;
}

void VideoSourceThread::run(){
	stepFrame();
}

void VideoSourceThread::threadRelease(){
	if (video.isOpened()) video.release();
}

void VideoSourceThread::stepFrame() {
	stepFrame(-1);
}

void VideoSourceThread::stepFrame(long frameIndex) {
	if (debug) {
      cout << "VideoSourceThread::run: width          " << *widthValue << endl;
      cout << "VideoSourceThread::run: height         " << *heightValue << endl;
   }

	if (!video.isOpened()) return;

	/* Set position in video file to specified index. 
		Negative index indicates no-spec. */
	if (frameIndex >= 0 && frameIndex != frameNumber) {
		video.set(CV_CAP_PROP_POS_FRAMES,frameIndex);
		cout << "Set frame " << frameIndex << "  " << frameNumber << "  " << imagePortOut->getName() << endl;
		frameNumber = frameIndex;
	}

	if (frameIndex == 0) {
		cout << "playback start" << endl;
		Bottle msg;
		msg.addString("playback");
		msg.addString("started");
		msg.addString(videoFile);
		events->write(msg);
	}

   /* Now try to get picture from the flv file*/
	ImageOf<PixelRgb> &outputImage = imagePortOut->prepare();

	/* read frame from the file*/
	cv::Mat frame;
	cv::Mat channel[3];
	video >> frame;
	frameNumber++;
		
	if (frame.empty()) {
		cout << "videoSource: playback finished at frame " << frameIndex << endl;
		Bottle msg;
		msg.addString("playback");
		msg.addString("finished");
		events->write(msg);
		if (*repeatPlayback && threadInit()) {
			video >> frame;
		} else {
			askToStop();
			return;
		}
	}

	/*resize for the output*/
	if (*widthValue > 0 && *heightValue > 0) {
		cv::Size newSize (*widthValue, *heightValue);
		cv::Mat resized;
		cv::resize(frame, resized, newSize);
		frame = resized;
	}

	IplImage * iplFrame;
	iplFrame = &frame.operator IplImage();
	IplImage* iplImage;

	outputImage.resize(iplFrame->width, iplFrame->height);
	iplImage = (IplImage*)outputImage.getIplImage();

	if (iplFrame->channelSeq[0] == 'B') {
		cvCvtColor(iplFrame, iplImage, CV_BGR2RGB);
	} else {
		cvCopy(iplFrame, iplImage, 0);
	}

	/* prepare frame number */
	VectorOf<long> &outputFrame = frameNumberOut->prepare();
	outputFrame.resize(1);
	outputFrame(0) = frameNumber;

	/*write result*/
	if (!imagePortOut->isClosed()) {
		imagePortOut->write();
		frameNumberOut->write();
	}
}

void HeartbeatListener::onRead(VectorOf<long>& data) {
	if (_playbackThread != NULL) _playbackThread->stepFrame(data[0]);
}

void HeartbeatListener::setThread(VideoSourceThread* thread) {
	_playbackThread = thread;
}