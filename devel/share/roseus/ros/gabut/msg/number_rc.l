;; Auto-generated. Do not edit!


(when (boundp 'gabut::number_rc)
  (if (not (find-package "GABUT"))
    (make-package "GABUT"))
  (shadow 'number_rc (find-package "GABUT")))
(unless (find-package "GABUT::NUMBER_RC")
  (make-package "GABUT::NUMBER_RC"))

(in-package "ROS")
;;//! \htmlinclude number_rc.msg.html


(defclass gabut::number_rc
  :super ros::object
  :slots (_rc_number ))

(defmethod gabut::number_rc
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

(setf (get gabut::number_rc :md5sum-) "2244ae5f9aac7ce5c8daccbb71337be0")
(setf (get gabut::number_rc :datatype-) "gabut/number_rc")
(setf (get gabut::number_rc :definition-)
      "int16 rc_number

")



(provide :gabut/number_rc "2244ae5f9aac7ce5c8daccbb71337be0")


