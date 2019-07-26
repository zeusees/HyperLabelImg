#ifndef __SFL_SEQUENCE_FACE_LANDMARKS__
#define __SFL_SEQUENCE_FACE_LANDMARKS__

// std
#include <string>
#include <list>
#include <memory>

// OpenCV
#include <opencv2/core.hpp>

namespace sfl
{
	/** @brief Represents a face detected in a frame.
	*/
    struct Face
    {
		int id;								///< Face id.
		cv::Rect bbox;						///< Bounding box.
        std::vector<cv::Point> landmarks;	///< Face landmarks.
    };

	/** @brief Represents a frame that might include faces.
	*/
    struct Frame
    {
		int id;									///< Frame id.
		int width;								///< Frame width [pixels]
		int height;								///< Frame height [pixels]
        std::list<std::unique_ptr<Face>> faces;	///< Detected faces in the frame

		/** @brief Get face by id.
		Return null if a face with the specified id is not found.
		*/
		const Face* getFace(int id) const;
    };

    /** @brief Represents face tracking type.
    */
    enum FaceTrackingType
    {
        TRACKING_NONE = 0,
        TRACKING_BRISK = 1,
        TRACKING_LBP = 2
    };


}   // namespace sfl

#endif	// __SFL_SEQUENCE_FACE_LANDMARKS__
