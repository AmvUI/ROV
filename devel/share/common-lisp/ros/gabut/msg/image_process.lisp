; Auto-generated. Do not edit!


(cl:in-package gabut-msg)


;//! \htmlinclude image_process.msg.html

(cl:defclass <image_process> (roslisp-msg-protocol:ros-message)
  ((state_red
    :reader state_red
    :initarg :state_red
    :type cl:fixnum
    :initform 0)
   (state_blue
    :reader state_blue
    :initarg :state_blue
    :type cl:fixnum
    :initform 0)
   (blue_crack
    :reader blue_crack
    :initarg :blue_crack
    :type cl:fixnum
    :initform 0))
)

(cl:defclass image_process (<image_process>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <image_process>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'image_process)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name gabut-msg:<image_process> is deprecated: use gabut-msg:image_process instead.")))

(cl:ensure-generic-function 'state_red-val :lambda-list '(m))
(cl:defmethod state_red-val ((m <image_process>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader gabut-msg:state_red-val is deprecated.  Use gabut-msg:state_red instead.")
  (state_red m))

(cl:ensure-generic-function 'state_blue-val :lambda-list '(m))
(cl:defmethod state_blue-val ((m <image_process>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader gabut-msg:state_blue-val is deprecated.  Use gabut-msg:state_blue instead.")
  (state_blue m))

(cl:ensure-generic-function 'blue_crack-val :lambda-list '(m))
(cl:defmethod blue_crack-val ((m <image_process>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader gabut-msg:blue_crack-val is deprecated.  Use gabut-msg:blue_crack instead.")
  (blue_crack m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <image_process>) ostream)
  "Serializes a message object of type '<image_process>"
  (cl:let* ((signed (cl:slot-value msg 'state_red)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'state_blue)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'blue_crack)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <image_process>) istream)
  "Deserializes a message object of type '<image_process>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'state_red) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'state_blue) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'blue_crack) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<image_process>)))
  "Returns string type for a message object of type '<image_process>"
  "gabut/image_process")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'image_process)))
  "Returns string type for a message object of type 'image_process"
  "gabut/image_process")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<image_process>)))
  "Returns md5sum for a message object of type '<image_process>"
  "ad9f5988e975c9e2128f2a0d58a5bb28")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'image_process)))
  "Returns md5sum for a message object of type 'image_process"
  "ad9f5988e975c9e2128f2a0d58a5bb28")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<image_process>)))
  "Returns full string definition for message of type '<image_process>"
  (cl:format cl:nil "int16 state_red~%int16 state_blue~%int16 blue_crack~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'image_process)))
  "Returns full string definition for message of type 'image_process"
  (cl:format cl:nil "int16 state_red~%int16 state_blue~%int16 blue_crack~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <image_process>))
  (cl:+ 0
     2
     2
     2
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <image_process>))
  "Converts a ROS message object to a list"
  (cl:list 'image_process
    (cl:cons ':state_red (state_red msg))
    (cl:cons ':state_blue (state_blue msg))
    (cl:cons ':blue_crack (blue_crack msg))
))
