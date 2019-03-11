; Auto-generated. Do not edit!


(cl:in-package gabut-msg)


;//! \htmlinclude mode.msg.html

(cl:defclass <mode> (roslisp-msg-protocol:ros-message)
  ((mode
    :reader mode
    :initarg :mode
    :type cl:fixnum
    :initform 0))
)

(cl:defclass mode (<mode>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <mode>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'mode)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name gabut-msg:<mode> is deprecated: use gabut-msg:mode instead.")))

(cl:ensure-generic-function 'mode-val :lambda-list '(m))
(cl:defmethod mode-val ((m <mode>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader gabut-msg:mode-val is deprecated.  Use gabut-msg:mode instead.")
  (mode m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <mode>) ostream)
  "Serializes a message object of type '<mode>"
  (cl:let* ((signed (cl:slot-value msg 'mode)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <mode>) istream)
  "Deserializes a message object of type '<mode>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'mode) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<mode>)))
  "Returns string type for a message object of type '<mode>"
  "gabut/mode")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'mode)))
  "Returns string type for a message object of type 'mode"
  "gabut/mode")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<mode>)))
  "Returns md5sum for a message object of type '<mode>"
  "7dcb05a3287517692aa8c4ee83b4e18b")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'mode)))
  "Returns md5sum for a message object of type 'mode"
  "7dcb05a3287517692aa8c4ee83b4e18b")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<mode>)))
  "Returns full string definition for message of type '<mode>"
  (cl:format cl:nil "int16 mode~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'mode)))
  "Returns full string definition for message of type 'mode"
  (cl:format cl:nil "int16 mode~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <mode>))
  (cl:+ 0
     2
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <mode>))
  "Converts a ROS message object to a list"
  (cl:list 'mode
    (cl:cons ':mode (mode msg))
))
