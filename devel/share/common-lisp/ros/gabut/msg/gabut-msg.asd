
(cl:in-package :asdf)

(defsystem "gabut-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "image_value" :depends-on ("_package_image_value"))
    (:file "_package_image_value" :depends-on ("_package"))
    (:file "number_rc" :depends-on ("_package_number_rc"))
    (:file "_package_number_rc" :depends-on ("_package"))
  ))