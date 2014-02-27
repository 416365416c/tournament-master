function deltaX(a,b){//a,b are items
    return (a.x+(a.width/2)) - (b.x+(b.width/2));
}
function deltaY(a,b){//a,b are items
    return (a.y+(a.height/2)) - (b.y+(b.height/2));
}
function mag(xArg,yArg){
    return Math.sqrt(xArg*xArg + yArg*yArg);
}
function angle(xArg,yArg){
    var ret = (Math.atan2(yArg,xArg) * 180)/Math.PI;
    if(ret < 0)
        ret = (ret+360);
    return ret;
}
