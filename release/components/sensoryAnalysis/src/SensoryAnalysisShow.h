#pragma once

#include <iostream>
#include<opencv2/opencv.hpp>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multiline_output.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>

using namespace cv;
using namespace std;

class MyImage : public Fl_Widget {
public:
	MyImage(int x, int y, int w, int h, char *label = 0);
	~MyImage();
	void draw();
	void GetImage(Mat &image);

private:
	Mat image;
	int Ix, Iy, ImageWidth, ImageHeight;
};

class PortGUI
{

public:
	PortGUI();
	~PortGUI();

	void init(string Path_C);
	bool show(Mat &I1_C, Mat &I2_C, Mat &I3_C, Mat &I4_C, Mat &I5_C, Mat &I6_C,
		Mat &I7_C, bool &KeyPreview_C, bool &KeyRecord_C, bool &Show3D_C, string &Time_C, double FPS_C,
		double MutualGaze_C, double SoundDirection_C, string &Voice_C, string &Speech_C, 
		int FaceID_C, int ExpressionID_C, int TrajectoryID_C);

	//control parameter
	bool exit_flag; //indicate if the exit button is pressed
	bool Show3DClickedFlag; //Once the 3
	bool flag_preview;
	bool flag_recording;
	bool flag_show3d;

	//my image container
	int ImageWidth, ImageHeight;
	Fl_RGB_Image *image_rgb;  //for drawing image
	MyImage *I1, *I2, *I3, *I4, *I5, *I6, *I7;

	//MainGUI components
	Fl_Window *MainWindow;
	Fl_Button *b_Preview, *b_Stop, *b_Record, *b_Exit, *b_Show3D;
	Fl_Group *GUI_Group;
	Fl_Box *box1_port, *box2_port, *box3_port, *box4_port, *box5_port, *box6_port, *box7_port, *_box_parameters, *_box_buttons,  *_box_OnlineRecord;
	Fl_PNG_Image *winbg, *_icon_Preview_play, *_icon_Preview_pause, *_icon_Stop, *_icon_Record, *_icon_Show3D, *_icon_Exit;
	Fl_Output *_edit_Time, *_edit_FPS, *_edit_MutualGaze, *_edit_Speech, *_edit_SoundDirection, *_edit_identifyVoice, *_edit_FaceID, *_edit_FaceExpressionID, *_edit_TrajectoryID;
	
	string Time_S, FPS_S, MutualGaze_S, Speech_S, SoundDirection_S, Voice_S, FaceID_S, ExpressionID_S, TrajectoryID_S;
	int display_x1, display_x2, display_x3, display_x4, display_x5, display_x6, display_x7, display_y1, display_y2; //image position
	int button_width, button_height; //button size
	int window_width, window_height; //main window size
	int edit_width, edit_height; //edit parameter size
};

