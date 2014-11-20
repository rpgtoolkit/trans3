//cnvTest = createcanvas(200, 200);
//colorRGB(255, 0, 0);
//fillrect(0, 0, 50, 50, cnvrendernow);
//setimage("shadow.png", 0, 0, 300, 200, cnvrendernow);
//setimage("shadow.png", 300, 200, 200, 200);
//drawcanvas(cnvrendernow, 0, 0);
//wait();
//rendernow(true);
width = 1;
height = 1;
colorRGB(255, 255, 255);
canvas = createCanvas(width, height);
while(true)
{
   fillRect(0, 0, width, height, canvas);
   drawCanvas(canvas, 0, 0, width, height);
   width += 0.4;
   height += 0.4;
}
