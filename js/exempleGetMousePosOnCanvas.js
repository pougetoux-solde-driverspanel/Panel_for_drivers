function getCoords(el,event) {
    var ox = -el.offsetLeft,
        oy = -el.offsetTop;
    while(el=el.offsetParent){
        ox += el.scrollLeft - el.offsetLeft;
        oy += el.scrollTop - el.offsetTop;
    }
    return {x:event.clientX + ox , y:event.clientY + oy};
}


$("#canvas").click(function(e)
{
		
	coords = getCoords(this,e);
}