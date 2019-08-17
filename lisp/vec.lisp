(defstruct vec 
  (x 0)
  (y 0)
  )
(defmacro vec-operator-macro (op p0 p1)
  `(progn
     (make-vec 
       :x (,op (vec-x ,p0) (vec-x ,p1))
       :y (,op (vec-y ,p0) (vec-y ,p1))
       )
     )
  )
(defun vec-add (p0 p1)
  (vec-operator-macro + p0 p1)
  )
