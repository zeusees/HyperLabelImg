/** @file
@brief Sequence face landmarks utility functions.
*/

#ifndef __SFL_UTILITIES__
#define __SFL_UTILITIES__

// sfl
#include "sequence_face_landmarks.h"

namespace sfl
{
	/** @brief Render landmarks.
	@param img The image that the landmarks will be rendered on.
	@param landmarks The landmark points to render.
	@param drawLabels if true, for each landmark, it's 0 based index will be
	rendererd as a label.
	@param color Line/point and label color.
	@param thickness Line/point thickness.
	*/
	void render(cv::Mat& img, const std::vector<cv::Point>& landmarks,
		bool drawLabels = false, const cv::Scalar& color = cv::Scalar(0, 255, 0),
		int thickness = 1);

	/** @brief Render bounding box.
	@param img The image that the bounding box will be rendered on.
	@param bbox The bounding box rectangle to render.
	@param color Line color.
	@param thickness Line thickness.
	*/
	void render(cv::Mat& img, const cv::Rect& bbox,
		const cv::Scalar& color = cv::Scalar(0, 255, 0), int thickness = 1);

	/** @brief Render face's bounding box and landmarks.
	@param img The image that the face will be rendered on.
	@param face The face to render.
    @param drawIDs if true, the 0 based id will be rendererd as a label.
	@param drawLabels if true, for each landmark, it's 0 based index will be
	rendererd as a label.
	@param bbox_color Bounding box line color.
	@param landmarks_color Landmarks line/point and label color.
	@param thickness Line/point thickness.
    @param fontScale The size of the font for the labels.
	*/
	void render(cv::Mat& img, const Face& face, bool drawIDs = true,
        bool drawLabels = false, const cv::Scalar& bbox_color = cv::Scalar(0, 0, 255),
		const cv::Scalar& landmarks_color = cv::Scalar(0, 255, 0), int thickness = 1,
		double fontScale = 1.0);

	/** @brief Render all frame faces including bounding boxs and landmarks.
	@param img The image that the faces will be rendered on.
	@param frame The frame to render.
    @param drawIDs if true, the 0 based id will be rendererd as a label.
	@param drawLabels if true, for each landmark, it's 0 based index will be
	rendererd as a label.
	@param bbox_color Bounding box line color.
	@param landmarks_color Landmarks line/point and label color.
	@param thickness Line/point thickness.
    @param fontScale The size of the font for the labels.
	*/
	void render(cv::Mat& img, const Frame& frame, bool drawIDs = true,
        bool drawLabels = false, const cv::Scalar& bbox_color = cv::Scalar(0, 0, 255),
		const cv::Scalar& landmarks_color = cv::Scalar(0, 255, 0), int thickness = 1,
		double fontScale = 1.0);

    void renderFaceID(cv::Mat& img, const Face& face, const cv::Scalar& color, int thickness = 1,
        double fontScale = 1.0);


	/** @brief Represents a face statistics in the sequence.
	*/
	struct FaceStat
	{
		int id = 0;					///< Face id.

		float avg_center_dist = 0;	///< Average distance from frame center.
		int frame_count = 0;		///< Frames appeared in.
		float avg_size = 0;			///< Average face's bounding box size.

		float central_ratio = 0;	///< avg_center_dist / (||(avg_width, avg_height)|| / 4).
		float frame_ratio = 0;		///< frame_count / total_frames
		float size_ratio = 0;		///< avg_size / ((avg_width + avg_height) / 4)
	};

	/** @brief Get the face statistics of the sequence
		@param sequence The sequence of frames to calculate the statistics for.
		@param stats Output vector of statistics for each face in the sequence.
	*/
	void getSequenceStats(const std::list<std::unique_ptr<Frame>>& sequence,
		std::vector<FaceStat>& stats);

	/** @brief Get the main face in a sequence.
	*/
	int getMainFaceID(const std::list<std::unique_ptr<Frame>>& sequence);

	/** @brief Get the main face from face statistics.
	*/
	int getMainFaceID(const std::vector<FaceStat>& stats);

    /** @brief Get the face's left eye center position (right eye in the image).
    @param landmarks 68 face points.
    */
    cv::Point2f getFaceLeftEye(const std::vector<cv::Point>& landmarks);

    /** @brief Get the face's right eye center position (left eye in the image).
    @param landmarks 68 face points.
    */
    cv::Point2f getFaceRightEye(const std::vector<cv::Point>& landmarks);

    /** @brief Get the face's vertical angle [radians].
    The angles are in the range [-75/180*pi, 75/180*pi].
    When the face is looking up the angle will be positive and when it is
    looking down it will be negative.
    @param landmarks 68 face points.
    */
    float getFaceApproxVertAngle(const std::vector<cv::Point>& landmarks);

    /** @brief Get the face's horizontal angle [radians].
    The angles are in the range [-75/180*pi, 75/180*pi].
    When the face is looking right (left in the image) the angle will be positive and
    when it is looking left (right in the image) it will be negative.
    @param landmarks 68 face points.
    */
    float getFaceApproxHorAngle(const std::vector<cv::Point>& landmarks);

    /** @brief Get the face's tilt angle [radians].
    The angles are in the range [-75/180*pi, 75/180*pi].
    When the face is tilting left (right in the image) the angle will be positive and
    when it is tilting right (left in the image) it will be negative.
    @param landmarks 68 face points.
    */
    float getFaceApproxTiltAngle(const std::vector<cv::Point>& landmarks);

    /** @brief Get the face's euler angles [radians].
    The angles are in the range [-75/180*pi, 75/180*pi].
    @param landmarks 68 face points.
    @return Return a vector with the 3 euler angles.
    The x axis represents vertical rotation angle, up is positive.
    The y axis represents horizontal rotation angle, right is positive.
    The z axis represents tilt rotation angle, left is positive.
    */
    cv::Point3f getFaceApproxEulerAngles(const std::vector<cv::Point>& landmarks);

    /** @brief Get face bounding box from landmarks.
    @param landmarks Face points.
    @param frameSize The size of the image.
    @param square Make the bounding box square (limited to frame boundaries).
    */
    cv::Rect getFaceBBoxFromLandmarks(const std::vector<cv::Point>& landmarks, 
        const cv::Size& frameSize, bool square);

    /** @brief Create full face points from landmarks.
    This will add the forehead part of the face.
    @param landmarks Face points.
    @param full_face The full face points.
    */
    void createFullFace(const std::vector<cv::Point>& landmarks,
        std::vector<cv::Point>& full_face);

}   // namespace sfl

#endif	// __SFL_UTILITIES__
