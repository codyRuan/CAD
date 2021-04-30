usage  
 $ make  
 $ ./PA3_106502013 testcase result  

testcase formate:  
INORDER = a b c d; //input variables  
OUTORDER = output; //output  
output = (!a*!b*!c*!d)+(!a*b*!c*d)+(!a*b*c*!d)+(a*!b*!c*d)+(a*!b*c*!d)+(a*b*!c*d)+  
(a*b*c*!d)+(a*b*c*d)+(!a*b*c*d) //non-minimized Boolean function  
  
output formate:  
INORDER = a b c d;  
OUTORDER = output;  
output = (!a*!b*!c*!d)+(a*!c*d)+(a*c*!d)+(b*d)+(b*c)
