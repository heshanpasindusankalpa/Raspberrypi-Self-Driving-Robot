import cv2
import numpy as np
import time

# Initialize Video File instead of PiCamera
video_path = "Road.mp4"  # Replace with your video file path
cap = cv2.VideoCapture(video_path)

kernal = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]], dtype=np.float64)

try:
    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break  # End of video

        image = frame.copy()
        h, w = image.shape[:2]
        frameCx = w // 2  # Center of the frame

        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        conv = cv2.filter2D(gray, -1, kernal)
        _, thres = cv2.threshold(conv, 100, 255, 0)

        # Find contours
        contours, _ = cv2.findContours(thres, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

        largest_contour = None
        max_area = 0

        for cnt in contours:
            area = cv2.contourArea(cnt)
            if area > 30 and area > max_area:  # Find the largest contour
                max_area = area
                largest_contour = cnt

        if largest_contour is not None:
            cv2.drawContours(image, [largest_contour], -1, (0, 255, 255), 3)
            rect = cv2.minAreaRect(largest_contour)
            box = cv2.boxPoints(rect)
            box = np.int0(box)
            cv2.drawContours(image, [box], 0, (0, 0, 255), 2)
            cv2.line(image, (frameCx, 0), (frameCx, h), (0, 255, 255), 2)

            M = cv2.moments(largest_contour)
            if M["m00"] != 0:
                cx = int(M["m10"] / M["m00"])
                cy = int(M["m01"] / M["m00"])
                cv2.line(image, (cx, cy), (frameCx, cy), (0, 255, 255), 2)

        # Show frames
        cv2.imshow("conv img", conv)
        cv2.imshow("thres img", thres)
        cv2.imshow("contours img", image)

        if cv2.waitKey(25) & 0xFF == ord('q'):
            break

finally:
    cap.release()
    cv2.destroyAllWindows()
    print("Resources released")
