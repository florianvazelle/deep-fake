#pragma once

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

// Credit : https://learnopencv.com/face-swap-using-opencv-c-python/

/**
 * @brief Applique une transformation affine
 */
static void applyAffineTransform(cv::Mat &warpImage, const cv::Mat &src, const std::vector<cv::Point2f> &srcTri, std::vector<cv::Point2f> &dstTri) {

    // Pour une paire de triangles, on trouve la transformation affine correspondante
    cv::Mat warpMat = cv::getAffineTransform(srcTri, dstTri);
    
    // On applique la transformation affine
    cv::warpAffine(src, warpImage, warpMat, warpImage.size(), cv::INTER_LINEAR, cv::BORDER_REFLECT_101);
}


/**
 * @brief Calculer les triangles de Delaunay
 */
static void calculateDelaunayTriangles(const cv::Rect& rect, const std::vector<cv::Point> &points, std::vector<std::vector<int>> &delaunayTri){

	// On crée une instance de Subdiv2D (qui sert est une subdivision de delaunay)
    cv::Subdiv2D subdiv(rect);

	// On ajoute les points à la subdivision
    for(std::vector<cv::Point>::const_iterator it = points.begin(); it != points.end(); it++)
        subdiv.insert(*it);	        

    // On récupère les triangles associés
	std::vector<cv::Vec6f> triangleList;
	subdiv.getTriangleList(triangleList);

    // Puis grâce à cette liste on va calculer une liste d'index correspondant aux points
	std::vector<cv::Point> pt(3);
	std::vector<int> ind(3);

	for(unsigned int i = 0; i < triangleList.size(); i++) {
		const cv::Vec6f& t = triangleList[i];
		pt[0] = cv::Point(t[0], t[1]);
		pt[1] = cv::Point(t[2], t[3]);
		pt[2] = cv::Point(t[4], t[5]);

		if (rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2])) {
			for(int j = 0; j < 3; j++)
				for(size_t k = 0; k < points.size(); k++)
					if(std::abs(pt[j].x - points[k].x) < 1.0 && abs(pt[j].y - points[k].y) < 1)						
						ind[j] = k;					

			delaunayTri.push_back(ind);
		}
	}
}

static bool roiCondition(const cv::Mat& m, const cv::Rect& roi) {
    return 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols && 0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows;
}

/**
 * @brief Déformation les triangulaires de img1 vers img2
 */
static void warpTriangle(const cv::Mat &img1, cv::Mat &img2, std::vector<cv::Point> &t1, std::vector<cv::Point> &t2) {
    
    cv::Rect r1 = cv::boundingRect(t1);
    cv::Rect r2 = cv::boundingRect(t2);

    // Décalage des points par le coin supérieur gauche des rectangles respectifs
    std::vector<cv::Point2f> t1Rect(3), t2Rect(3);
    std::vector<cv::Point> t2RectInt(3);
    for(int i = 0; i < 3; i++) {
        t1Rect[i] = cv::Point2f(t1[i].x - r1.x, t1[i].y -  r1.y);
        t2Rect[i] = cv::Point2f(t2[i].x - r2.x, t2[i].y - r2.y);

        t2RectInt[i] = cv::Point(t2[i].x - r2.x, t2[i].y - r2.y);
    }
    
    // On calcule le mask correspondant au triangle
    cv::Mat mask = cv::Mat::zeros(r2.height, r2.width, CV_32FC3);
    cv::fillConvexPoly(mask, t2RectInt, cv::Scalar(1.0, 1.0, 1.0), 16, 0);
    
    // On applique la transformation affine aux sous parties des images, correspondant au carré englobant le triangle
    cv::Mat img1Rect;
    img1(r1).copyTo(img1Rect);
    
    cv::Mat img2Rect = cv::Mat::zeros(r2.height, r2.width, img1Rect.type());
    
    applyAffineTransform(img2Rect, img1Rect, t1Rect, t2Rect);
    
    cv::multiply(img2Rect, mask, img2Rect);
    cv::multiply(img2(r2), cv::Scalar(1.0, 1.0, 1.0) - mask, img2(r2));
    img2(r2) = img2(r2) + img2Rect;
}

static void draw_delaunay(cv::Mat& img, const std::vector<cv::Point> &points) {
    cv::Size size = img.size();
    cv::Rect rect(0, 0, size.width, size.height);

    cv::Subdiv2D subdiv(rect);

    for (std::vector<cv::Point>::const_iterator it = points.begin(); it != points.end(); it++)
        subdiv.insert(*it);

    std::vector<cv::Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    std::vector<cv::Point> pt(3);

    for (size_t i = 0; i < triangleList.size(); i++) {
        cv::Vec6f t = triangleList[i];
        pt[0] = cv::Point(cvRound(t[0]), cvRound(t[1]));
        pt[1] = cv::Point(cvRound(t[2]), cvRound(t[3]));
        pt[2] = cv::Point(cvRound(t[4]), cvRound(t[5]));

        if (rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2])) {
            cv::line(img, pt[0], pt[1], cv::Scalar(255, 0, 0), 1, cv::LINE_AA, 0);
            cv::line(img, pt[1], pt[2], cv::Scalar(255, 0, 0), 1, cv::LINE_AA, 0);
            cv::line(img, pt[2], pt[0], cv::Scalar(255, 0, 0), 1, cv::LINE_AA, 0);
        }
    }
}