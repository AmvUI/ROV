;; Auto-generated. Do not edit!


(when (boundp 'gabut::image_value)
  (if (not (find-package "GABUT"))
    (make-package "GABUT"))
  (shadow 'image_value (find-package "GABUT")))
(unless (find-package "GABUT::IMAGE_VALUE")
  (make-package "GABUT::IMAGE_VALUE"))

(in-package "ROS")
;;//! \htmlinclude image_value.msg.html


(defclass gabut::image_value
  :super ros::object
  :slots (_state_red _state_blue _blue_crack ))

(defmethod gabut::image_value
  (:init
   (&key
    ((:state_red __state_red) 0)
    ((:state_blue __state_blue) 0)
    ((:blue_crack __blue_crack) 0)
    )
   (send-super :init)
   (setq _state_red (round __state_red))
   (setq _state_blue (round __state_blue))
   (setq _blue_crack (round __blue_crack))
   self)
  (:state_red
   (&optional __state_red)
   (if __state_red (setq _state_red __state_red)) _state_red)
  (:state_blue
   (&optional __state_blue)
   (if __state_blue (setq _state_blue __state_blue)) _state_blue)
  (:blue_crack
   (&optional __blue_crack)
   (if __blue_crack (setq _blue_crack __blue_crack)) _blue_crack)
  (:serialization-length
   ()
   (+
    ;; int16 _state_red
    2
    ;; int16 _state_blue
    2
    ;; int16 _blue_crack
    2
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int16 _state_red
       (write-word _state_red s)
     ;; int16 _state_blue
       (write-word _state_blue s)
     ;; int16 _blue_crack
       (write-word _blue_crack s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int16 _state_red
     (setq _state_red (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _state_blue
     (setq _state_blue (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _blue_crack
     (setq _blue_crack (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;;
   self)
  )

(setf (get gabut::image_value :md5sum-) "ad9f5988e975c9e2128f2a0d58a5bb28")
(setf (get gabut::image_value :datatype-) "gabut/image_value")
(setf (get gabut::image_value :definition-)
      "int16 state_red
int16 state_blue
int16 blue_crack

")



(provide :gabut/image_value "ad9f5988e975c9e2128f2a0d58a5bb28")


