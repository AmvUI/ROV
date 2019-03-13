; Auto-generated. Do not edit!


(cl:in-package gabut-msg)


;//! \htmlinclude number_rc.msg.html

(cl:defclass <number_rc> (roslisp-msg-protocol:ros-message)
  ((rc_number
    :reader rc_number
    :initarg :rc_number
    :type cl:fixnum
    :initform 0))
)

(cl:defclass number_rc (<number_rc>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <number_rc>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'number_rc)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name gabut-msg:<number_rc> is deprecated: use gabut-msg:number_rc instead.")))

(cl:ensure-generic-function 'rc_number-val :lambda-list '(m))
(cl:defmethod rc_number-val ((m <number_rc>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader gabut-msg:rc_number-val is deprecated.  Use gabut-msg:rc_number instead.")
  (rc_number m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <number_rc>) ostream)
  "Serializes a message object of type '<number_rc>"
  (cl:let* ((signed (cl:slot-value msg 'rc_number)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <number_rc>) istream)
  "Deserializes a message object of type '<number_rc>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'rc_number) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<number_rc>)))
  "Returns string type for a message object of type '<number_rc>"
  "gabut/number_rc")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'number_rc)))
  "Returns string type for a message object of type 'number_rc"
  "gabut/number_rc")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<number_rc>)))
  "Returns md5sum for a message object of type '<number_rc>"
  "2244ae5f9aac7ce5c8daccbb71337be0")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'number_rc)))
  "Returns md5sum for a message object of type 'number_rc"
  "2244ae5f9aac7ce5c8daccbb71337be0")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<number_rc>)))
  "Returns full string definition for message of type '<number_rc>"
  (cl:format cl:nil "int16 rc_number~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'number_rc)))
  "Returns full string definition for message of type 'number_rc"
  (cl:format cl:nil "int16 rc_number~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <number_rc>))
  (cl:+ 0
     2
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <number_rc>))
  "Converts a ROS message object to a list"
  (cl:list 'number_rc
    (cl:cons ':rc_number (rc_number msg))
))
