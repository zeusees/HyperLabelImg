#include "utilities.h"

// std
#include <map>

// OpenCV
#include <opencv2/imgproc.hpp>

using std::runtime_error;

const float MAX_FACE_ANGLE = 75.0f;

namespace sfl
{
	void render(cv::Mat & img, const std::vector<cv::Point>& landmarks,
		bool drawLabels, const cv::Scalar & color, int thickness)
	{
		if (landmarks.size() == 68)
		{
			for (size_t i = 1; i <= 16; ++i)
				cv::line(img, landmarks[i], landmarks[i - 1], color, thickness);

			for (size_t i = 28; i <= 30; ++i)
				cv::line(img, landmarks[i], landmarks[i - 1], color, thickness);

			for (size_t i = 18; i <= 21; ++i)
				cv::line(img, landmarks[i], landmarks[i - 1], color, thickness);

			for (size_t i = 23; i <= 26; ++i)
				cv::line(img, landmarks[i], landmarks[i - 1], color, thickness);

			for (size_t i = 31; i <= 35; ++i)
				cv::line(img, landmarks[i], landmarks[i - 1], color, thickness);
			cv::line(img, landmarks[30], landmarks[35], color, thickness);

			for (size_t i = 37; i <= 41; ++i)
				cv::line(img, landmarks[i], landmarks[i - 1], color, thickness);
			cv::line(img, landmarks[36], landmarks[41], color, thickness);

			for (size_t i = 43; i <= 47; ++i)
				cv::line(img, landmarks[i], landmarks[i - 1], color, thickness);
			cv::line(img, landmarks[42], landmarks[47], color, thickness);

			for (size_t i = 49; i <= 59; ++i)
				cv::line(img, landmarks[i], landmarks[i - 1], color, thickness);
			cv::line(img, landmarks[48], landmarks[59], color, thickness);

			for (size_t i = 61; i <= 67; ++i)
				cv::line(img, landmarks[i], landmarks[i - 1], color, thickness);
			cv::line(img, landmarks[60], landmarks[67], color, thickness);
		}
		else
		{
			for (size_t i = 0; i < landmarks.size(); ++i)
				cv::circle(img, landmarks[i], thickness, color, -1);
		}

		if (drawLabels)
		{
			// Add labels
			for (size_t i = 0; i < landmarks.size(); ++i)
				cv::putText(img, std::to_string(i), landmarks[i],
					cv::FONT_HERSHEY_PLAIN, 0.5, color, thickness);
		}
	}

	void render(cv::Mat& img, const cv::Rect& bbox, const cv::Scalar& color,
		int thickness)
	{
		cv::rectangle(img, bbox, color, thickness);
	}

	void render(cv::Mat& img, const Face& face, bool drawIDs, bool drawLabels,
		const cv::Scalar& bbox_color, const cv::Scalar& landmarks_color, int thickness,
		double fontScale)
	{
		render(img, face.bbox, bbox_color, thickness);
		render(img, face.landmarks, drawLabels, landmarks_color, thickness);

		// Add face id label
        if (drawIDs) renderFaceID(img, face, bbox_color, thickness, fontScale);
	}

	void render(cv::Mat& img, const Frame& frame, bool drawIDs, bool drawLabels,
		const cv::Scalar& bbox_color, const cv::Scalar& landmarks_color, int thickness,
		double fontScale)
	{
		for (auto& face : frame.faces)
			render(img, *face, drawIDs, drawLabels, bbox_color, landmarks_color, thickness,
				fontScale);
	}

    void renderFaceID(cv::Mat& img, const Face& face, const cv::Scalar& color,
        int thickness, double fontScale)
    {
        std::string lbl = std::to_string(face.id);
        int baseline = 0;
        cv::Size textSize = cv::getTextSize(lbl, cv::FONT_HERSHEY_PLAIN,
            fontScale, thickness, &baseline);
        cv::Point lbl_pt(face.bbox.x + (face.bbox.width - textSize.width) / 2,
            face.bbox.y - textSize.height / 4);
        cv::putText(img, std::to_string(face.id), lbl_pt,
            cv::FONT_HERSHEY_PLAIN, fontScale, color, thickness);
    }

	void getSequenceStats(const std::list<std::unique_ptr<Frame>>& sequence,
		std::vector<FaceStat>& stats)
	{
		std::map<int, int> face_map;
		int total_frames = 0;
		cv::Point2f center, pos;
		float dist, max_dist = 0;
		float size, max_size = 0;
		float avg_frame_width = 0, avg_frame_height = 0;

		// For each frame
		for (auto& frame : sequence)
		{
			if (frame->faces.empty()) continue;
			++total_frames;

			center.x = frame->width*0.5f;
			center.y = frame->height*0.5f;
			avg_frame_width += (float)frame->width;
			avg_frame_height += (float)frame->height;

			// For each face
			for (auto& face : frame->faces)
			{
				// Get face stat
				int i = face_map[face->id];
				if (i >= stats.size() || face->id != stats[i].id)
				{
					// Create new face stat
					stats.push_back(FaceStat());
					i = stats.size() - 1;
					face_map[face->id] = i;
					stats[i].id = face->id;
				}
				FaceStat& face_stat = stats[i];

				// Add center distance
				cv::Point tl = face->bbox.tl();
				cv::Point br = face->bbox.br();
				pos.x = (tl.x + br.x)*0.5f;
				pos.y = (tl.y + br.y)*0.5f;
				dist = (float)cv::norm(pos - center);
				face_stat.avg_center_dist += dist;

				// Increase frame count
				++(face_stat.frame_count);

				// Add face size
				size = (face->bbox.width + face->bbox.height)*0.5f;
				face_stat.avg_size += size;

			}
		}

		if (total_frames == 0) return;

		// Calculate averages and ranges
		avg_frame_width /= total_frames;
		avg_frame_height /= total_frames;
		max_dist = 0.25f*std::sqrt(avg_frame_width*avg_frame_width +
			avg_frame_height*avg_frame_height);
		max_size = 0.25f*(avg_frame_width + avg_frame_height);

		for (auto& stat : stats)
		{
			stat.avg_center_dist /= stat.frame_count;
			stat.avg_size /= stat.frame_count;
		}

		// Finalize stats
		for (auto& stat : stats)
		{
			// Calculate central ratio
			if (max_dist < 1e-6f) stat.central_ratio = 1.0f;
			else stat.central_ratio = (1 - stat.avg_center_dist / max_dist);
			stat.central_ratio = std::min(std::max(0.0f, stat.central_ratio), 1.0f);

			// Calculate frame ratio
			stat.frame_ratio = float(stat.frame_count) / total_frames;

			// Calculate size ratio
			if (max_size < 1e-6f) stat.size_ratio = 1.0f;
			else stat.size_ratio = stat.avg_size / max_size;
			stat.size_ratio = std::min(std::max(0.0f, stat.size_ratio), 1.0f);
		}
	}

	int getMainFaceID(const std::list<std::unique_ptr<Frame>>& sequence)
	{
		std::vector<FaceStat> stats;
		getSequenceStats(sequence, stats);
		return getMainFaceID(stats);
	}

	int getMainFaceID(const std::vector<FaceStat>& stats)
	{
		int best_id = -1;
		float score, best_score = 0;

		// For each stat
		for (auto& stat : stats)
		{
			score = (stat.central_ratio + stat.frame_ratio + stat.size_ratio) / 3;
			if (score > best_score)
			{
				best_score = score;
				best_id = stat.id;
			}
		}

		return best_id;
	}

    cv::Point2f getFaceLeftEye(const std::vector<cv::Point>& landmarks)
    {
        if (landmarks.size() != 68) return cv::Point2f();

        cv::Point2f left_eye(0, 0);
        for (size_t i = 42; i <= 47; ++i)
            left_eye += cv::Point2f(landmarks[i]);

        return (left_eye / 6);
    }

    cv::Point2f getFaceRightEye(const std::vector<cv::Point>& landmarks)
    {
        if (landmarks.size() != 68) return cv::Point2f();

        cv::Point2f right_eye(0, 0);
        for (size_t i = 36; i <= 41; ++i)
            right_eye += cv::Point2f(landmarks[i]);

        return (right_eye / 6);
    }

    float getFaceApproxVertAngle(const std::vector<cv::Point>& landmarks)
    {
        if (landmarks.size() != 68) return 0;
        cv::Point2f left_eye = getFaceLeftEye(landmarks);
        cv::Point2f right_eye = getFaceRightEye(landmarks);
        cv::Point2f x1 = landmarks[0], x2 = landmarks[16];
        cv::Point2f v = x2 - x1;
        cv::Point2f right_eye_dir = x1 - right_eye;
        cv::Point2f left_eye_dir = x1 - left_eye;
        float x12_dist = cv::norm(v);
        float d1 = v.cross(right_eye_dir) / x12_dist;
        float d2 = v.cross(left_eye_dir) / x12_dist;
        float d = (d1 + d2)*0.5f / cv::norm(left_eye - right_eye);
        return d * (2 * MAX_FACE_ANGLE) * (CV_PI / 180.0f);
    }

    float getFaceApproxHorAngle(const std::vector<cv::Point>& landmarks)
    {
        if (landmarks.size() != 68) return 0;
        const float max_angle = 75.0f;

        const cv::Point& center = landmarks[27];
        const cv::Point& left_eye = landmarks[42];
        const cv::Point& right_eye = landmarks[39];
        float left_dist = cv::norm(center - left_eye);
        float right_dist = cv::norm(center - right_eye);
        float d = (left_dist / (left_dist + right_dist) - 0.5f);

        return d * (2 * MAX_FACE_ANGLE) * (CV_PI / 180.0f);
    }

    float getFaceApproxTiltAngle(const std::vector<cv::Point>& landmarks)
    {
        if (landmarks.size() != 68) return 0;

        cv::Point2f left_eye = getFaceLeftEye(landmarks);
        cv::Point2f right_eye = getFaceRightEye(landmarks);
        cv::Point2f v = left_eye - right_eye;
        return atan2(v.y, v.x);
    }

    cv::Point3f getFaceApproxEulerAngles(const std::vector<cv::Point>& landmarks)
    {
        float x = getFaceApproxVertAngle(landmarks);
        float y = getFaceApproxHorAngle(landmarks);
        float z = getFaceApproxTiltAngle(landmarks);

        return cv::Point3f(x, y, z);
    }

	cv::Rect getFaceBBoxFromLandmarks(const std::vector<cv::Point>& landmarks,
		const cv::Size& frameSize, bool square)
	{
		int xmin = 10000, ymin = 10000;
		int	xmax = -1, ymax = -1;
		for (const cv::Point& p : landmarks)
		{
			xmin = std::min(xmin, p.x);
			ymin = std::min(ymin, p.y);
			xmax = std::max(xmax, p.x);
			ymax = std::max(ymax, p.y);

		}

		int width = xmax - xmin;
		int height = ymax - ymin;
		int centerx = (xmin + xmax) / 2;
		int centery = (ymin + ymax) / 2;

		int tsize = std::max(width, height) / 2;

		//float rescaleCASIA[4] = { 1.785974, 1.951171, 1.835600, 1.670403 }
		float rescaleCASIA[4] = { 1.9255, 2.0591, 1.9255, 1.6087 };;
		//float rescaleCASIA[4] = { 1.9255, 2.2591, 1.9423, 1.6087 };
		int dleft = round( centerx - rescaleCASIA[0] * tsize);
        int dtop = round(centery - rescaleCASIA[1] * tsize);
        int dright = round(centerx + rescaleCASIA[2] * tsize);
        int dbottom = round(centery + rescaleCASIA[3] * tsize);

      
		int nw = int(dright - dleft);
		int nh = int(dbottom - dtop);
		if (dleft < 0) dleft = 0;
		if (dright > frameSize.width) dright = frameSize.width;
		if (dtop < 0) dtop = 0;
		if (dbottom > frameSize.height) dbottom = frameSize.height;
       

        return cv::Rect(cv::Point(dleft, dtop), cv::Point(dright, dbottom));
    }

    void createFullFace(const std::vector<cv::Point>& landmarks, std::vector<cv::Point>& full_face)
    {
        if (landmarks.size() != 68) return;

        // Jaw line
        full_face = {
            { landmarks[0] },
            { landmarks[1] },
            { landmarks[2] },
            { landmarks[3] },
            { landmarks[4] },
            { landmarks[5] },
            { landmarks[6] },
            { landmarks[7] },
            { landmarks[8] },
            { landmarks[9] },
            { landmarks[10] },
            { landmarks[11] },
            { landmarks[12] },
            { landmarks[13] },
            { landmarks[14] },
            { landmarks[15] },
            { landmarks[16] }
        };

        // Forehead
        cv::Point dir = (landmarks[27] - landmarks[30]);
        if (landmarks[26].x > landmarks[16].x) full_face.push_back(landmarks[26]);
        full_face.push_back(landmarks[26] + dir);
        full_face.push_back(landmarks[24] + dir);
        full_face.push_back(landmarks[19] + dir);
        full_face.push_back(landmarks[17] + dir);
        if (landmarks[17].x < landmarks[0].x) full_face.push_back(landmarks[17]);
    }

}   // namespace sfl

