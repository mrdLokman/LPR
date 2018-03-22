#include"display.h"
//----------------------------
cv::Mat display_images(std::vector<cv::Mat>& vecMat, int windowHeight, int nRows) {
	int N = vecMat.size();
	nRows = nRows > N ? N : nRows;
	int edgeThickness = 5;
	int imagesPerRow = ceil(double(N) / nRows);
	int resizeHeight =  floor(2.0 * ((floor(double(windowHeight - edgeThickness) / nRows)) / 2.0)) - edgeThickness;
	int maxRowLength = 0;

	std::vector<int> resizeWidth;
	for (int i = 0; i < N;) {
		int thisRowLen = 0;
		for (int k = 0; k < imagesPerRow; k++) {
			double aspectRatio = double(vecMat[i].cols) / vecMat[i].rows;
			int temp = int(vecMat[i].cols);// (ceil(resizeHeight * aspectRatio));
			resizeWidth.push_back(temp);
			//resizeWidth.push_back((double)vecMat[i].cols);
			thisRowLen += temp;
			if (++i == N) break;
		}
		if ((thisRowLen + edgeThickness * (imagesPerRow + 1)) > maxRowLength) {
			maxRowLength = thisRowLen + edgeThickness * (imagesPerRow + 1);
		}
	}
	int windowWidth = maxRowLength;
	cv::Mat canvasImage(windowHeight, windowWidth, CV_8UC3, Scalar(255, 0, 0));
   int max=0;int h = edgeThickness;
	for (int k = 0, i = 0; i < nRows; i++) {
		
		int y = i * resizeHeight + (i + 1) * edgeThickness; 
		
		 max+=  edgeThickness+ h;
		 h = edgeThickness;
		int x_end = edgeThickness;
		for (int j = 0; j < imagesPerRow && k < N; k++, j++) {
			int x = x_end;
			y = max;
			h = h > (int)vecMat[k].rows ? h : (int)vecMat[k].rows;
			cv::Rect roi(x, y, resizeWidth[k], (int)vecMat[k].rows);
			cv::Size s = canvasImage(roi).size();
			// change the number of channels to three
			cv::Mat target_ROI(s, CV_8UC3);
			if (vecMat[k].channels() != canvasImage.channels()) {
				if (vecMat[k].channels() == 1) {
					cv::cvtColor(vecMat[k], target_ROI, CV_GRAY2BGR);
				}
			}
			else {
				vecMat[k].copyTo(target_ROI);
			}
			cv::resize(target_ROI, target_ROI, s);
			if (target_ROI.type() != canvasImage.type()) {
				target_ROI.convertTo(target_ROI, canvasImage.type());
			}
			target_ROI.copyTo(canvasImage(roi));
			x_end += resizeWidth[k] + edgeThickness;
		}
	}
	return canvasImage;
}

void ShowManyImages(string title, int nArgs, ...) {
	int size;
	int i;
	int m, n;
	int x, y;

	// w - Maximum number of images in a row
	// h - Maximum number of images in a column
	int w, h;

	// scale - How much we have to resize the image
	float scale;
	int max;

	// If the number of arguments is lesser than 0 or greater than 12
	// return without displaying
	if (nArgs <= 0) {
		printf("Number of arguments too small....\n");
		return;
	}
	else if (nArgs > 14) {
		printf("Number of arguments too large, can only handle maximally 12 images at a time ...\n");
		return;
	}
	// Determine the size of the image,
	// and the number of rows/cols
	// from number of arguments
	else if (nArgs == 1) {
		w = h = 1;
		size = 300;
	}
	else if (nArgs == 2) {
		w = 2; h = 1;
		size = 300;
	}
	else if (nArgs == 3 || nArgs == 4) {
		w = 2; h = 2;
		size = 300;
	}
	else if (nArgs == 5 || nArgs == 6) {
		w = 3; h = 2;
		size = 200;
	}
	else if (nArgs == 7 || nArgs == 8) {
		w = 4; h = 2;
		size = 200;
	}
	else {
		w = 4; h = 3;
		size = 150;
	}

	// Create a new 3 channel image
	Mat DispImage = Mat::zeros(Size(100 + size*w, 60 + size*h), CV_8UC3);

	// Used to get the arguments passed
	va_list args;
	va_start(args, nArgs);

	// Loop for nArgs number of arguments
	for (i = 0, m = 20, n = 20; i < nArgs; i++, m += (20 + size)) {
		// Get the Pointer to the IplImage
		Mat img = va_arg(args, Mat);

		// Check whether it is NULL or not
		// If it is NULL, release the image, and return
		if (img.empty()) {
			printf("Invalid arguments");
			return;
		}

		// Find the width and height of the image
		x = img.cols;
		y = img.rows;

		// Find whether height or width is greater in order to resize the image
		max = (x > y) ? x : y;

		// Find the scaling factor to resize the image
		scale = (float)((float)max / size);

		// Used to Align the images
		if (i % w == 0 && m != 20) {
			m = 20;
			n += 20 + size;
		}

		// Set the image ROI to display the current image
		// Resize the input image and copy the it to the Single Big Image
		Rect ROI(m, n, (int)(x / scale), (int)(y / scale));
		Mat temp; resize(img, temp, Size(ROI.width, ROI.height));
		temp.copyTo(DispImage(ROI));
	}

	// Create a new window, and show the Single Big Image
	namedWindow(title, 1);
	imshow(title, DispImage);
	waitKey();

	// End the number of arguments
	va_end(args);
}