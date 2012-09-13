#include <iostream>
#include <stdio.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include <sys/time.h>
using namespace std;
#define WIDTH 1000
#define HEIGHT 1000
#define e 0.6666
#define RESISTANCE 0.0245
#define pi 3.141592653
float Xcord, Ycord, Xzoom, Yzoom;
float tankX;  // tank X co-ordinates tank onluymoves in x axis
float gun;  // gun angle
int power;// power of the shot  level to signify the level of the stage
GLUquadricObj *ball_d[34]; // the diferent balls  ball 30,31,32,33 are the gold balls the target balls
int mass_flag;  // signifying which ball was used
timeval t0;  // will store the result of gettimeofday always will b the latest time
double t1;    // store the previous value of time
typedef struct ball {
	float mass;
	float x, y ;
	float vx, vy;
	double time;
}ball;
struct ball all_ball[34];     // all the balls info is in this array 
int active_ball[34], current;  // value is 1 of the ball still active current is the index of ball just launched
// level data
float level,wave1,wave2,wave3,wave4;
int flag[4], tcR,tcB,tcG, score;
GLUquadricObj *gold[4]; // the target balls
//struct ball gold_ball[4]; // their characteristics

void display(){
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	glClear( GL_COLOR_BUFFER_BIT );
	glPushMatrix();

	char sc[100];
	sprintf(sc,"TRIES LEFT");
	glRasterPos3d(10,890 ,0.0);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(unsigned char*)sc );


	glColor3f(1,0,0);   // red ball count
	sprintf(sc,"%d",tcR);
	glRasterPos3d(10,850 ,0.0);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(unsigned char*)sc );
	// making the tank base here info regarding the number of balls left will be written

	glColor3f(0,1,0);
	sprintf(sc,"%d",tcG);  // green ball count
	glRasterPos3d(10,800 ,0.0);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(unsigned char*)sc );

	glColor3f(0,0,1);
	sprintf(sc,"%d",tcB);  // blue ball count
	glRasterPos3d(10,750 ,0.0);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(unsigned char*)sc );

	glColor3f(1,0,0);   // score
	sprintf(sc,"%d",score);
	glRasterPos3d(950,950 ,0.0);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(unsigned char*)sc );

	// Ball chosen
	glPushMatrix();
	glTranslatef(40,HEIGHT-60,0);  // making the display for the type of ball to be used
	GLUquadricObj *ball_type;
	ball_type = gluNewQuadric();
	if(mass_flag == 5){
		glColor3f(1,0,0);
	}else if(mass_flag == 10){
		glColor3f(0,1,0);
	}else if(mass_flag == 15){
		glColor3f(0,0,1);
	} 
	gluDisk(ball_type,0,20,32,32);
	glPopMatrix();
	// Power meter
	glColor3f(0.33,0.33,1);
	glBegin(GL_QUADS);
	glVertex2f(0,HEIGHT-25);
	glVertex2f(0,HEIGHT);
	glVertex2f(power,HEIGHT);
	glVertex2f(power,HEIGHT-25);
	glEnd();
	// power meter made
	// the base
	glColor3f(1,0.5,0.1); 
	glBegin(GL_QUADS);
	glVertex2f(0,0);
	glVertex2f(300,0);
	glVertex2f(300,100);
	glVertex2f(0,100);
	glEnd();

	// making the tank

	glTranslatef(tankX,100,0);
	glColor3f(0.112,0.219,0.147); // the tyres part
	glBegin(GL_QUADS);
	glVertex2f(25,0);
	glVertex2f(75,0);
	glVertex2f(100,25);
	glVertex2f(0,25);
	glEnd();

	//glPushMatrix();
	glTranslatef(50,25,0); // the upper part of the tank
	glColor3f(0.34,0.139,0.34);
	glBegin(GL_LINE_LOOP);
	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	gluPartialDisk(quadratic,20,35,32,32,-90,180);
	//glPushMatrix(); // the new axis where the rotation will occur
	glRotatef(gun,0,0,1);
	glTranslatef(33,0,0);
	glColor3f(0.34,0.139,0.34);
	glBegin(GL_QUADS);
	glVertex2f(0,0);
	glVertex2f(15,0);
	glVertex2f(15,15);  // 15 is the diameter of the ball
	glVertex2f(0,15);
	glEnd();
	glTranslatef(15,15,0);	 // the mouth of the tiger
	//glPopMatrix();	
	//glPopMatrix();	
	glPopMatrix();	
	for(i=0;i<30;i++){
		if(active_ball[i]==1){
			glPushMatrix();
			glTranslatef(all_ball[i].x,all_ball[i].y,0);
			if(all_ball[i].mass == 5){
				glColor3f(1,0,0);
			}else if(all_ball[i].mass == 10){
				glColor3f(0,1,0);
			}else if(all_ball[i].mass == 15){
				glColor3f(0,0,1);
			} 
			glBegin(GL_POLYGON);
			for(int i=0;i<360;i++)
			{
				glVertex2f(7.5*cos(i*(pi/180)),7.5*sin(i*(pi/180)));  // radius = 7.5
			}
			glEnd();
		
			glPopMatrix();
		}	
	}
	// obstacles making   // design # 1 wavy moving // level variable will make it difficult or easier // width of each level=100
	// hence we get a max space of 7 wavelets but with a 50 separation - we will have 4 waves	
	//the first wave is small max height = 100 cordinates are 350 to 450
	glColor3f(0.153,0.51,0.0);
	glBegin(GL_QUADS);
	glVertex2f(350,0);
	glVertex2f(450,0);
	glVertex2f(450,wave1);  
	glVertex2f(350,wave1);
	glEnd();


	//the second wave is bigger max height = 300 cordinates are 500 to 600
	glColor3f(0.153,0.51,0.0);
	glBegin(GL_QUADS);
	glVertex2f(500,0);
	glVertex2f(600,0);
	glVertex2f(600,wave2);  
	glVertex2f(500,wave2);
	glEnd();


	//the third wave is bigger max height = 600 cordinates are 650 to 750
	glColor3f(0.153,0.51,0.0);
	glBegin(GL_QUADS);
	glVertex2f(650,0);
	glVertex2f(750,0);
	glVertex2f(750,wave3);  
	glVertex2f(650,wave3);
	glEnd();

	//the fourtht wave is biggest max height = 900 cordinates are 800 to 900
	glColor3f(0.153,0.51,0.0);
	glBegin(GL_QUADS);
	glVertex2f(800,0);
	glVertex2f(900,0);
	glVertex2f(900,wave4);  
	glVertex2f(800,wave4);
	glEnd();

	for(i=0;i<4;i++){
		glPushMatrix();
		glTranslatef(all_ball[30+i].x,all_ball[30+i].y,0);
	
		glColor3f(1,0,1);
	
		glBegin(GL_POLYGON);
		for(int i=0;i<360;i++)
		{
			glVertex2f(7.5*cos(i*(3.14/180)),7.5*sin(i*(3.14/180)));
		}
		glEnd();
		glPopMatrix();
	}	
	glutSwapBuffers();
}

	void keyboard( unsigned char key, int x, int y ){
		if( key == 27)
			exit(0);
		if( key == 'a'){
			//cout << "a is pressed\n";
			if(power > 5)
				power += -5;
		}
		if (key == 'd'){
		//	cout << "d is pressed\n";
			if(power < 150 )
				power += 5;
		}
		if (key == 's'){
		//	cout << "s is pressed\n";
			int i;
			for(i=0;i<30;i++){
				if(active_ball[i]==0){
					current = i;
					if(tcR>0 && mass_flag==5.0){
						tcR -= 1;
					}else if (tcG > 0 && mass_flag==10.0){
						tcG -= 1;
					}else if (tcB > 0 && mass_flag==15.0){
						tcB -= 1;
					}else{
						break;
					}
					//position
					all_ball[i].x=48.0*cos(gun*(pi/180.0)) - 7.5*sin(gun*(pi/180.0)) + tankX + 50.0;
					all_ball[i].y=48.0*sin(gun*(pi/180.0)) + 7.5*cos(gun*(pi/180.0)) + 125.0;  
					all_ball[i].mass = mass_flag;
					all_ball[i].vx = ((power*5.0)/mass_flag)*cos(gun*(pi/180.0)) ;
					all_ball[i].vy = ((power*5.0)/mass_flag)*sin(gun*(pi/180.0)) ;
					gettimeofday(&t0,NULL);       // getting the time
					all_ball[i].time=t0.tv_sec+(t0.tv_usec/1000000.0);
					active_ball[i] = 1;
					break;
				}
			}
		}
		if (key == '1')
			mass_flag = 5.0;
		if (key == '2')
			mass_flag = 10.0;
		if (key == '3')
			mass_flag = 15.0;
		if (key == 'l')      /// the mass of target balls increases, the height and speed of the movement of the waves
			level += 0.3;
		if( key == 'o'){
			if(level > 0.0)
				level -= 0.3;
		}
		if (key == ',')
			glViewport(Xcord++,Ycord,Xzoom,Yzoom);
		if (key == '.')
			glViewport(Xcord--,Ycord,Xzoom,Yzoom);
		if (key == 'n')
			glViewport(Xcord,Ycord++,Xzoom,Yzoom);
		if (key == 'm')
			glViewport(Xcord,Ycord--,Xzoom,Yzoom);
		if (key == '=')
			glViewport(Xcord,Ycord,Xzoom--,Yzoom--);
		if (key == '-')
			glViewport(Xcord,Ycord,Xzoom++,Yzoom++);
	}

// the tank cant be allowed to fall off the base
// also the nozzle cant go inside the tank :P
// hence a limit to the x cord and gun angle

void tank_movement( int key, int, int ){
	if (key == GLUT_KEY_LEFT) {
		if(tankX > 0)
			tankX -= 1;
	}
	if (key == GLUT_KEY_RIGHT) {
		if(tankX < 225)
			tankX += 1;
	}
	if (key == GLUT_KEY_UP) {
		if(gun < 160)
			gun += 1;
	}
	if (key == GLUT_KEY_DOWN) {
		if(gun>0)
			gun -= 1;
	}
}

void idle( void ){
	// the trajectory of the active balls	
	gettimeofday(&t0,NULL);
	double tt = t0.tv_sec + (t0.tv_usec/1000000.0);
	int i,done[34];
	int j;
	for(j=0;j<34;j++)           
		done[j]=0;
	for(i=0;i<34;i++){
		if(active_ball[i]==1 && done[i]==0){           // one ball collides once only
		//	cout << i << "outside ball is i\n"; 
			for(j=i+1;j<34;j++){
				if(active_ball[j]==1 && done[j]==0){
					float x_dif,y_dif;
					x_dif = (all_ball[i].x - all_ball[j].x);
					y_dif = (all_ball[i].y - all_ball[j].y);
											 // distance between the centres at collision = 225
					if(((x_dif * x_dif) + (y_dif * y_dif)) <= 230){  // remember radius of ball = 7.5
		//				cout << j << "collided ball is j\n"; 
		//				cout << i << "collided ball is i\n"; 
						done[i] = 1;                        
						done[j] = 1;
						//vector angle					arctan(m or slope )
						//equation for normal direction			vaf = ((e+1)*mb*(vbi)n + (vai)n*(ma - e*mb))/(ma + mb)
						//						vbf = ((e+1)*ma*(vai)n + (vbi)n*(mb – e*ma))/(ma + mb)
						//	float slope = y_dif/x_dif ;
						float theta = atan(y_dif/x_dif);
						float tan_vi = all_ball[i].vy*cos(theta)-all_ball[i].vx*sin(theta);
						float tan_vj = all_ball[j].vy*cos(theta)-all_ball[j].vx*sin(theta);
						float norm_vi = all_ball[i].vx*cos(theta) + all_ball[i].vx*sin(theta);
						float norm_vj = all_ball[j].vx*cos(theta) + all_ball[j].vx*sin(theta);
						float norm_vjf= all_ball[i].mass*norm_vi*(e+1); 
						norm_vjf += (all_ball[j].mass-(e*all_ball[i].mass))*norm_vj ;
						norm_vjf = norm_vjf/(all_ball[i].mass+all_ball[j].mass);
						float norm_vif=all_ball[j].mass*norm_vj*(e+1); 
						norm_vif += (all_ball[i].mass-(e*all_ball[j].mass))*norm_vi ;
						norm_vif = norm_vif/(all_ball[i].mass+all_ball[j].mass);
						all_ball[i].vx = tan_vi*cos(theta)-norm_vif*sin(theta);
						all_ball[j].vx = tan_vj*cos(theta)-norm_vjf*sin(theta);
						all_ball[i].vy = tan_vi*sin(theta)+norm_vif*cos(theta);
						all_ball[j].vy = tan_vj*sin(theta)+norm_vjf*cos(theta);

						// calculating score
						if(i>=30 ){
							score += all_ball[j].mass*10;
						}
						if(j>=30 ){
							score += all_ball[i].mass*10;
						}
						//int flagG = 0;   // flag for the gold/target balls
						if(all_ball[i].time==0){
							//flag=1;   // flag for i
							all_ball[i].time=all_ball[j].time;  // now this is the time these balls start moving
							if(all_ball[i].vy < 0)
								all_ball[i].vy *= -1;
						}
						if(all_ball[j].time==0){
							//flag = 2 ; // flag for j
							all_ball[j].time=all_ball[i].time;
							if(all_ball[j].vy < 0)
								all_ball[j].vy *= -1;
						}
						x_dif = (all_ball[i].x - all_ball[j].x);
						y_dif = (all_ball[i].y - all_ball[j].y);
						double t = 0.001;
						// updating the values of ball's x and y cord so that they dont appear stuck
						// also when a collision occurs there is impulse force which changes their velocity
						// in very less time hence changing their positions by a very small factor
						// which is denoted by t
						while(((x_dif * x_dif) + (y_dif * y_dif)) <= 240){
							all_ball[i].x += all_ball[i].vx*t;
							all_ball[i].y += all_ball[i].vy*t;
							all_ball[j].x += all_ball[j].vx*t;
							all_ball[j].y += all_ball[j].vy*t;
							x_dif = (all_ball[i].x - all_ball[j].x);
							y_dif = (all_ball[i].y - all_ball[j].y);

						}

					}

				}
			}
		}
	}
	double t = 0.005;   // time step for changing velocities by some magnitude after collisions 
	// t is a necessary evil - else when the next time idle is called
	// the velocities are updated before hence by the time idle gets called and 
	// collision with the base and other pillars has already happened and the projectiles move
	// very little in each call of idle(as this function is called many times in a second) therefore we see
	// the projectiles stuck or to pass through the walls or each other
	for(i=0;i<34;i++){
		if(active_ball[i]==1){
			if(all_ball[i].y <= 107.5 && all_ball[i].x <= 300){   // THE BASE
				all_ball[i].vy = -1*e*all_ball[i].vy;		// 100 + 7.5 <- radius
				all_ball[i].x += all_ball[i].vx*t;
				all_ball[i].y += all_ball[i].vy*t;
				continue;
			}

			// first block
			if(all_ball[i].y <= wave1 && ((350 - all_ball[i].x) <= 7.5) && ((350 - all_ball[i].x) >= 0) ){   
				all_ball[i].vx = -1*e*all_ball[i].vx;
				all_ball[i].x += all_ball[i].vx*t;
				all_ball[i].y += all_ball[i].vy*t;
				continue;
			}
			if(all_ball[i].y <= wave1 && ((all_ball[i].x - 450) <= 7.5) && ((all_ball[i].x - 450) >= 0) ){   
				all_ball[i].vx = -1*e*all_ball[i].vx;
				all_ball[i].x += all_ball[i].vx*t;
				all_ball[i].y += all_ball[i].vy*t;
				continue;
			}
			if(((all_ball[i].y-wave1) <= 7.5) && ((all_ball[i].y-wave1) >= 0) && ((450 - all_ball[i].x) <= 100 ) && ((all_ball[i].x - 350) <= 100)){  
				all_ball[i].vy = -e*all_ball[i].vy;
				all_ball[i].x += all_ball[i].vx*t;
				all_ball[i].y += all_ball[i].vy*t+level*5*flag[0];
				continue;
			}
			// second block
			if(all_ball[i].y <= wave2 && ((500 - all_ball[i].x) <= 7.5) && ((500 - all_ball[i].x) >= 0) ){   
				all_ball[i].vx = -1*e*all_ball[i].vx;
				all_ball[i].x += all_ball[i].vx*t;
				all_ball[i].y += all_ball[i].vy*t;
				continue;
			}
			if(all_ball[i].y <= wave2 && ((all_ball[i].x - 600) <= 7.5) && ((all_ball[i].x - 600) >= 0) ){   
				all_ball[i].vx = -1*e*all_ball[i].vx;
				all_ball[i].x += all_ball[i].vx*t;
				all_ball[i].y += all_ball[i].vy*t;
				continue;
			}
			if(((all_ball[i].y-wave2) <= 7.5) && ((all_ball[i].y-wave2) >= 0) && ((600 - all_ball[i].x) <= 100 ) && ((all_ball[i].x - 500) <= 100)){  
				all_ball[i].vy = -e*all_ball[i].vy;
				all_ball[i].x += all_ball[i].vx*t;
				all_ball[i].y += all_ball[i].vy*t + level*5*flag[1];
				continue;
			}
			// third block
			if(all_ball[i].y <= wave3 && ((650 - all_ball[i].x) <= 7.5) && ((650 - all_ball[i].x) >= 0) ){   
				all_ball[i].vx = -1*e*all_ball[i].vx;
				all_ball[i].x += all_ball[i].vx*t;
				all_ball[i].y += all_ball[i].vy*t;
				continue;
			}
			if(all_ball[i].y <= wave3 && ((all_ball[i].x - 750) <= 7.5) && ((all_ball[i].x - 750) >= 0) ){   
				all_ball[i].vx = -1*e*all_ball[i].vx;
				all_ball[i].x += all_ball[i].vx*t;
				all_ball[i].y += all_ball[i].vy*t;
				continue;
			}
			if(((all_ball[i].y-wave3) <= 7.5) && (all_ball[i].y-wave3 >= 0) && ((750 - all_ball[i].x) <= 100 ) && ((all_ball[i].x - 650) <= 100)){  
				all_ball[i].vy = -e*all_ball[i].vy;
				all_ball[i].x += all_ball[i].vx*t;
				all_ball[i].y += all_ball[i].vy*t + level*5*flag[2];
				continue;
			}
			// fourth block
			if(all_ball[i].y <= wave4 && ((800 - all_ball[i].x) <= 7.5) && ((800 - all_ball[i].x) >= 0) ){   
				all_ball[i].vx = -1*e*all_ball[i].vx;
				all_ball[i].x += all_ball[i].vx*t;
				all_ball[i].y += all_ball[i].vy*t;
				continue;
			}
			if(all_ball[i].y <= wave4 && ((all_ball[i].x - 900) <= 7.5) && ((all_ball[i].x - 900) >= 0) ){   
				all_ball[i].vx = -1*e*all_ball[i].vx;
				all_ball[i].x += all_ball[i].vx*t;
				all_ball[i].y += all_ball[i].vy*t;
				continue;
			}
			if(((all_ball[i].y-wave4) <= 7.5) && ((all_ball[i].y - wave4) >= 0) && ((900 - all_ball[i].x) <= 100 ) && ((all_ball[i].x - 800) <= 100)){  
				all_ball[i].vy = -e*all_ball[i].vy;
				all_ball[i].x += all_ball[i].vx*t;
				all_ball[i].y += all_ball[i].vy*t + level*5*flag[3];
				continue;
			}

		}
	}

	if((wave1+(level*5*flag[0])) < 100 && (wave1+(level*5*flag[0])) > 0){
		wave1 += level*5*flag[0];
		if(all_ball[30].vx == 0 && all_ball[30].vy == 0)
			all_ball[30].y = wave1+7.5;
	}else{
		flag[0] *=-1;
		wave1 += level*5*flag[0];
		if(all_ball[30].vx == 0 && all_ball[30].vy == 0)
			all_ball[30].y = wave1+7.5;
	}

	if((wave2+(level*5*flag[1])) < 600 && (wave2+(level*5*flag[1])) > 0){
		wave2 += level*5*flag[1];
		if(all_ball[31].vx == 0 && all_ball[31].vy == 0)
			all_ball[31].y = wave2+7.5;
	}else{
		flag[1] *= -1;
		wave2 += level*5*flag[1];
		if(all_ball[31].vx == 0 && all_ball[31].vy == 0)
			all_ball[31].y = wave2+7.5;
	}

	if((wave3+(level*5*flag[2])) < 300 && (wave3+(level*5*flag[2])) > 0){
		wave3 += level*5*flag[2];
		if(all_ball[32].vx == 0 && all_ball[32].vy == 0)
			all_ball[32].y = wave3+7.5;
	}else{
		flag[2] *= -1;
		wave3 += level*5*flag[2];
		if(all_ball[32].vx == 0 && all_ball[32].vy == 0)
			all_ball[32].y = wave3+7.5;
	}

	if((wave4+(level*5*flag[3])) < 900 && (wave4+(level*5*flag[3])) > 0){
		wave4 += level*5*flag[3];
		if(all_ball[33].vx == 0 && all_ball[33].vy == 0)
			all_ball[33].y = wave4+7.5;
	}else{
		flag[3] *= -1;
		wave4 += level*5*flag[3];
		if(all_ball[33].vx == 0 && all_ball[33].vy == 0)
			all_ball[33].y = wave4+7.5;
	}


	for(i=0;i<30;i++){
		if(active_ball[i]==1){
			double t = tt - all_ball[i].time;
			if(t<=0)
				continue;
			all_ball[i].x += all_ball[i].vx*t - all_ball[i].vx*RESISTANCE*t*t/(2*all_ball[i].mass);
			all_ball[i].y += all_ball[i].vy*t - (5.0)*t*t  - all_ball[i].vy*RESISTANCE*t*t/(2*all_ball[i].mass);
			all_ball[i].vx += -RESISTANCE*t*all_ball[i].vx/all_ball[i].mass ;
			all_ball[i].vy += -10.0*t - RESISTANCE*t*all_ball[i].vx/all_ball[i].mass ;
			all_ball[i].time = tt;
			if(all_ball[i].x > 1000.0 || all_ball[i].x  < 0 || all_ball[i].y < 0)
				active_ball[i] = 0;
			if(all_ball[i].x < 300.0 && all_ball[i].y < 100.0 )
				active_ball[i] = 0;
		}
	}
	for ( i=30; i<34 ; i++){
		if(all_ball[i].time != 0.0){
			double t = tt - all_ball[i].time;
			all_ball[i].x += all_ball[i].vx*t;
			all_ball[i].y += all_ball[i].vy*t - (5.0)*t*t;
			all_ball[i].vy += -10.0*t ;
			all_ball[i].time = tt;
		}
	}
	display();
}
int pressed; // flag for the mouse pressed  b1 = 1 -> b2 =2 -> b1 b2 =3
void mouse(int button, int state, int x, int y){
		if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON)
			pressed = 2;
		if (state == GLUT_DOWN && button == GLUT_MIDDLE_BUTTON)
			pressed = 3;
		if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
			pressed = 1;
		if(state == GLUT_UP && button == GLUT_RIGHT_BUTTON ){
			pressed = 0;
			if(tankX > 0)
				tankX -= 1;
		}
		if(button==3){
			Xzoom -= button;
			Yzoom -= button;
			glViewport(Xcord,Ycord,Xzoom,Yzoom);
		}
		if(button==4){
			Xzoom += button;
			Yzoom += button;
			glViewport(Xcord,Ycord,Xzoom++,Yzoom++);
		}
		if(state == GLUT_UP && button == GLUT_LEFT_BUTTON ){
			pressed = 0;
			if(tankX < 225)
				tankX += 1;
		}
		if(state == GLUT_UP && button == GLUT_MIDDLE_BUTTON ){
			pressed = 0;
			int i;
			for(i=0;i<30;i++){
				if(active_ball[i]==0){
					current = i;
					if(tcR>0 && mass_flag==5.0){
						tcR -= 1;
					}else if (tcG > 0 && mass_flag==10.0){
						tcG -= 1;
					}else if (tcB > 0 && mass_flag==15.0){
						tcB -= 1;
					}else{
						break;
					}
					//position
					all_ball[i].x=48.0*cos(gun*(pi/180.0)) - 7.5*sin(gun*(pi/180.0)) + tankX + 50.0;
					all_ball[i].y=48.0*sin(gun*(pi/180.0)) + 7.5*cos(gun*(pi/180.0)) + 125.0;
					all_ball[i].mass = mass_flag;
					all_ball[i].vx = ((power*5.0)/mass_flag)*cos(gun*(pi/180.0)) ;
					all_ball[i].vy = ((power*5.0)/mass_flag)*sin(gun*(pi/180.0)) ;
					gettimeofday(&t0,NULL);       // getting the time
					all_ball[i].time=t0.tv_sec+(t0.tv_usec/1000000.0);
					active_ball[i] = 1;
					break;
				}
			}
		}
}


int xini = 0, xfin =0 , yini = 0 , yfin = 0;
void mousemotion(int x,int y){
	xfin = x;
	yfin = y;
	if(pressed == 3){
		if(yfin > yini && gun > 0){
			gun -= 1;
		}  else if(yfin < yini && gun < 160){
			gun += 1;
		} 
		if(xfin > xini && power < 150)
			power += 1;
		else if(xfin < xini && power > 5)
			power -= 1;
	}

	if(pressed == 1){
		if (xfin>xini)
			glViewport(Xcord++,Ycord,Xzoom,Yzoom);
		if (xfin<xini)
			glViewport(Xcord--,Ycord,Xzoom,Yzoom);
		if (yfin<yini)
			glViewport(Xcord,Ycord++,Xzoom,Yzoom);
		if (yfin>yini)
			glViewport(Xcord,Ycord--,Xzoom,Yzoom);
	}

	xini = xfin;
	yini = yfin;
}

	void mouseWheel(int button, int dir, int x, int y){
		if (dir > 0){ 
			Xzoom -= button;
			Yzoom -= button;
			glViewport(Xcord,Ycord,Xzoom,Yzoom);
		}else{
			Xzoom += button;
			Yzoom += button;
			glViewport(Xcord,Ycord,Xzoom++,Yzoom++);
		}
	}

int main(int argc, char **argv ){
	// initialising the global charactersistics
	int i;
	for(i=0;i<30;i++){
		active_ball[i]=0; // no ball shot at first
	}
	// level data
	active_ball[30]=1;  // the 4 balls 
	active_ball[31]=1;
	active_ball[32]=1;
	active_ball[33]=1;
	tcB = 5 ; 	
	tcG = 10 ; 	
	tcR = 15 ; 	
	wave1 = 10;
	wave2 = 10;
	wave3 = 10;
	wave4 = 10;
	all_ball[30].x=400.0; // first ball
	all_ball[30].y=wave1+7.5;
	all_ball[30].vx=0.0;
	all_ball[30].vy=0.0;
	all_ball[30].mass=10;
	all_ball[30].time=0;

	all_ball[31].x=550.0;      // second ball
	all_ball[31].y=wave2+7.5;
	all_ball[31].vx=0.0;
	all_ball[31].vy=0.0;
	all_ball[31].mass=10.0;
	all_ball[31].time=0.0;

	all_ball[32].x=700.0;
	all_ball[32].y=wave3+7.5;
	all_ball[32].vx=0.0;
	all_ball[32].vy=0.0;
	all_ball[32].mass=10.0;
	all_ball[32].time=0.0;

	all_ball[33].x=850;
	all_ball[33].y=wave4+7.5;
	all_ball[33].vx=0.0;
	all_ball[33].vy=0.0;
	all_ball[33].mass=10.0;
	all_ball[33].time=0.0;

	for(i=0;i<4;i++)
		flag[i]=1;
	mass_flag=5;
	current = 0; 
	tankX = 0;  // value cant exceed 225
	level=1.2;
	gun = 0;  // value cant be more than 180
	power = 5; // default value
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( WIDTH, HEIGHT );
	glutCreateWindow( "Transformations in 2D" );
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	Xcord=0;
	Ycord=0;
	Xzoom=WIDTH;
	Yzoom=HEIGHT;
	glViewport( 0, 0, WIDTH, HEIGHT );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D ( 0, WIDTH, 0, HEIGHT );

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glutKeyboardFunc( keyboard );
	glutSpecialFunc( tank_movement );
	glutMouseFunc( mouse );
	glutMotionFunc( mousemotion );
	glutMouseWheelFunc(mouseWheel);
	glutMainLoop();
	return 0;
}
