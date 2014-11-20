//Start up sequence
autolocal(1);
brd = createCanvas(640,480);
canvasGetScreen(brd);
cnv = createCanvas(153, 153);

bitmap("stat_freija_face.gif", cnv);
drawCanvas(cnv, 467, 100);
bold(1);
mWin("Emeraude")
bold(0);
mWin("Ahgh!")
delay(.05);
wait();
mWin("I wonder if the caravan has arrived yet...");
delay(.05);
wait();

drawCanvas(brd,0,0);
mWincls();

playerDirection(0, tkDIR_W);
itemDirection(0,tkDIR_E);
canvasGetScreen(brd);

bitmap("stat_freija_face.gif", cnv);
drawCanvas(cnv, 21, 100);
bold(1);
mWin("Tao");
bold(0);
mWin("Morning Emeraude. I'm hungry. I want fish.")
mp3pause("fight.wav");
wait();
mWin("Now.");
delay(.05);
wait();

