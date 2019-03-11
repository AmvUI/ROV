;; Auto-generated. Do not edit!


(when (boundp 'gabut::mode)
  (if (not (find-package "GABUT"))
    (make-package "GABUT"))
  (shadow 'mode (find-package "GABUT")))
(unless (find-package "GABUT::MODE")
  (make-package "GABUT::MODE"))

(in-package "ROS")
;;//! \htmlinclude mode.msg.html


(defclass gabut::mode
  :super ros::object
  :slots (_mode ))

(defmethod gabut::mode
  (:init
   (&key
    ((:mode __mode) 0)
    )
   (send-super :init)
   (setq _mode (round __mode))
   self)
  (:mode
   (&optional __mode)
   (if __mode (setq _mode __mode)) _mode)
  (:serialization-length
   ()
   (+
    ;; int16 _mode
    2
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int16 _mode
       (write-word _mode s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int16 _mode
     (setq _mode (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;;
   self)
  )

(setf (get gabut::mode :md5sum-) "7dcb05a3287517692aa8c4ee83b4e18b")
(setf (get gabut::mode :datatype-) "gabut/mode")
(setf (get gabut::mode :definition-)
      "int16 mode

")



(provide :gabut/mode "7dcb05a3287517692aa8c4ee83b4e18b")


