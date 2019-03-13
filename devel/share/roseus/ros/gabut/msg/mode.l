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
  :slots (_rc_number ))

(defmethod gabut::mode
  (:init
   (&key
    ((:rc_number __rc_number) 0)
    )
   (send-super :init)
   (setq _rc_number (round __rc_number))
   self)
  (:rc_number
   (&optional __rc_number)
   (if __rc_number (setq _rc_number __rc_number)) _rc_number)
  (:serialization-length
   ()
   (+
    ;; int16 _rc_number
    2
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int16 _rc_number
       (write-word _rc_number s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int16 _rc_number
     (setq _rc_number (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;;
   self)
  )

(setf (get gabut::mode :md5sum-) "2244ae5f9aac7ce5c8daccbb71337be0")
(setf (get gabut::mode :datatype-) "gabut/mode")
(setf (get gabut::mode :definition-)
      "int16 rc_number

")



(provide :gabut/mode "2244ae5f9aac7ce5c8daccbb71337be0")


