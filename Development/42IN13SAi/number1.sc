func Main()
[
	var t = 60;
	var s = 1000;
   var v = velo(s|t);
   
   prnt(v);
   prnt(time(s|v));
   prnt(dist(v|t));
   
   var dx = 56;
   var dv = 30;
   var dt = 10;
   
   prnt(accl(dv|dt));
   prnt(avgv(dx|dt));  

]