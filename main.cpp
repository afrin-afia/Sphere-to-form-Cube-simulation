#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
int countHomekey=0;
struct point
{
	double x,y,z;
};
struct vector{

    public:
    double x,y,z;

    vector findPerp(vector v){          //this and v er perp ber kroe return kore
        struct vector a;
        a.x= (y*v.z) - (z*v.y);
        a.y= (z*v.x) - (x*v.z);
        a.z= (z*v.y) - (y*v.x);
        return a;

    }


    vector rotate(  vector ref,  float theta){       //ths k ref er reference e theta deg ghurao 3D rot

            struct vector ans;
            struct vector perp= findPerp(ref);
            ans.x=perp.x * sin(theta) + x*cos(theta);
            ans.y=perp.y * sin(theta) + y*cos(theta);
            ans.z=perp.z * sin(theta) + z*cos(theta);
            return ans;
    }

    void copy( vector v2){          //v2 k this e copy koro
                float v2val= sqrt((v2.x)*(v2.x)+(v2.y)*(v2.y)+(v2.z)*(v2.z));
                x= v2.x/v2val;
                y= v2.y/v2val;
                z= v2.z/v2val;

    }

};

double Mcubelen=20.0, crad,ch, Curcubelen=14.0;

//for camera:
struct point pos;       //camera position
struct vector u,r,l;


void drawAxes()
{
	if(drawaxes==1)
	{
        //glTranslatef(0,0,Mcubelen/2);
		glBegin(GL_LINES);{
		    glColor3f(1.0, 0.0, 0.0);
			glVertex3f( 100,0,Mcubelen);
			glVertex3f(-100,0,Mcubelen);

            glColor3f(0.0, 1.0, 0.0);
			glVertex3f(0,-100,Mcubelen);
			glVertex3f(0, 100,Mcubelen);

            glColor3f(0.0, 0.0, 1.0);
			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				//glVertex3f(i*10, -90, 0);
				//glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				//glVertex3f(-90, i*10, 0);
				//glVertex3f( 90, i*10, 0);

				//z axis
				glVertex3f(-200, 0, i*20);
				glVertex3f( 200, 0, i*20);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(0.2,0.2,0.2);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCylinderPart(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments/4;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_QUADS);
        {
            //glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,height);
			glVertex3f(points[i].x,points[i].y,height);

        }
        glEnd();
    }
}


void drawSpherePart(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	int count=0;
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);

		for(j=0;j<slices/4;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                //glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				//glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}



void keyboardListener(unsigned char key, int x,int y){
	switch(key){

        struct vector newl,newr,newu;
		case '2':
			//rotate camera right

            newl= l.rotate(u,.250);
            newr= r.rotate(u,.250);
            l.copy(newl);           //newl k l e rakho
            r.copy(newr);
			break;

        case '1':
			//rotate camera right
            newl= l.rotate(u,-.250);
            newr= r.rotate(u,-.250);
            l.copy(newl);           //newl k l e rakho
            r.copy(newr);
			break;

        case '4':
			//look up
            newl= l.rotate(r,.250);
            newu= u.rotate(r,.250);
            l.copy(newl);
            u.copy(newu);
			break;

        case '3':
			//look down
            newl= l.rotate(r,-.250);
            newu= u.rotate(r,-.250);
            l.copy(newl);           //newl k l e rakho
            u.copy(newu);
			break;

        case '6':
			//tilt clockwise
            newu= u.rotate(l,.250);
            newr= r.rotate(l,.250);
            u.copy(newu);           //newl k l e rakho
            r.copy(newr);
			break;

        case '5':
			//tilt c.clokwise
            newu= u.rotate(l,-.250);
            newr= r.rotate(l,-.250);
            u.copy(newu);           //newl k l e rakho
            r.copy(newr);
			break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key, move back
			cameraHeight -= 3.0;
			pos.x-= l.x*3;
			pos.y-= l.y*3;
			pos.z-= l.z*3;
			break;
		case GLUT_KEY_UP:		// up arrow key, move forward
			cameraHeight += 3.0;
			pos.x+= l.x*3;
			pos.y+= l.y*3;
			pos.z+= l.z*3;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			pos.x+= r.x*3;
			pos.y+= r.y*3;
			pos.z+= r.z*3;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			pos.x-= r.x*3;
			pos.y-= r.y*3;
			pos.z-= r.z*3;
			break;

		case GLUT_KEY_PAGE_UP:
            pos.x+= u.x*3;
			pos.y+= u.y*3;
			pos.z+= u.z*3;
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos.x-= u.x*3;
			pos.y-= u.y*3;
			pos.z-= u.z*3;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:         //cube to sphere
            Curcubelen-=2.0;
            countHomekey+=3;
            if(Curcubelen< 0) {
                Curcubelen=0.0;
                countHomekey-=3;
            }
			break;
		case GLUT_KEY_END:
            Curcubelen+=2.0;
            countHomekey-=3;
            if(Curcubelen> Mcubelen){
                 Curcubelen= Mcubelen;
                 countHomekey+=3;
            }
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}


////////////************************START**********************

void drawCube(){

    crad= Mcubelen-Curcubelen;         //?
    ch=Curcubelen;

    glColor3f(1,1,1);
    glPushMatrix();{            //roof
        glTranslatef(0,0,2*Mcubelen);
        drawSquare(Curcubelen);
    }glPopMatrix();

    glPushMatrix();{            //mejhe

        drawSquare(Curcubelen);
    }glPopMatrix();

    glPushMatrix();{            //right
        glTranslatef(0,Mcubelen+2,Mcubelen+2);
        glRotatef(90,1,0,0);
        drawSquare(Curcubelen);
    }glPopMatrix();

    glPushMatrix();{            //left
        glTranslatef(0,-Mcubelen+2,Mcubelen+2);
        glRotatef(90,1,0,0);
        drawSquare(Curcubelen);
    }glPopMatrix();

    glPushMatrix();{
        glTranslatef(Mcubelen-2,0,Mcubelen+2);
        glRotatef(90,0,0,1);
        glRotatef(90,1,0,0);
        drawSquare(Curcubelen);
    }glPopMatrix();

    glPushMatrix();{
        glTranslatef(-Mcubelen-2,0,Mcubelen+2);
        glRotatef(90,0,0,1);
        glRotatef(90,1,0,0);
        drawSquare(Curcubelen);
    }glPopMatrix();


    ///cylinders

    glPushMatrix();{                //lomba 1
        glTranslatef(Curcubelen,Curcubelen,countHomekey+1+Curcubelen/2);
        drawCylinderPart(crad,2*ch,20);
    }glPopMatrix();

     glPushMatrix();{               //lomba 2
        glTranslatef(-Curcubelen,Curcubelen,countHomekey+1+Curcubelen/2);
        glRotatef(90,0,0,1);
        drawCylinderPart(crad,2*ch,20);
    }glPopMatrix();

    //lomba 3
    glPushMatrix();{                //from lomba 1: 180 rotate
        glTranslatef(-Curcubelen,-Curcubelen,countHomekey+1+Curcubelen/2);
        glRotatef(180,0,0,1);
        drawCylinderPart(crad,2*ch,20);
    }glPopMatrix();

    //lomba 4
    glPushMatrix();{               //from lomba 2
        glTranslatef(Curcubelen,-Curcubelen,countHomekey+1+Curcubelen/2);
        glRotatef(180,0,0,1);
        glRotatef(90,0,0,1);
        drawCylinderPart(crad,2*ch,20);
    }glPopMatrix();


    //shouano 1
     glPushMatrix();{
        glTranslatef(-Curcubelen,Curcubelen,countHomekey+1+Curcubelen/2);
        glRotatef(90,0,1,0);
        drawCylinderPart(crad,2*ch,20);
    }glPopMatrix();

     glPushMatrix();{       //shoua 2
        glTranslatef(-Curcubelen,-Curcubelen,countHomekey+1+Curcubelen/2);
        glRotatef(90,0,0,1);
        glRotatef(90,0,1,0);
        drawCylinderPart(crad,2*ch,20);
    }glPopMatrix();

    glPushMatrix();{            //from shoua 1
        glTranslatef(Curcubelen,-Curcubelen,countHomekey+1+Curcubelen/2);
        glRotatef(180,0,0,1);
        glRotatef(90,0,1,0);
        drawCylinderPart(crad,2*ch,20);
    }glPopMatrix();

     glPushMatrix();{       //from shoua 2
        glTranslatef(Curcubelen,Curcubelen,countHomekey+1+Curcubelen/2);
        glRotatef(180,0,0,1);
        glRotatef(90,0,0,1);
        glRotatef(90,0,1,0);
        drawCylinderPart(crad,2*ch,20);
    }glPopMatrix();

    //from here uporer shoua gula (4Ta)
     glPushMatrix();{               //shua 4
        glTranslatef(-Curcubelen,Curcubelen,2+crad+Curcubelen*2);
         glRotatef(90,1,0,0);
        glRotatef(90,0,1,0);
        drawCylinderPart(crad,2*ch,20);
    }glPopMatrix();

    //s5
    glPushMatrix();{              //frm shua 4
        glTranslatef(Curcubelen,-Curcubelen,2+crad+Curcubelen*2);
        glRotatef(180,0,0,1);
        glRotatef(90,1,0,0);
        glRotatef(90,0,1,0);
        drawCylinderPart(crad,2*ch,20);
    }glPopMatrix();

     //shua 6
     glPushMatrix();{              //frm shua 5
        glTranslatef(Curcubelen,Curcubelen,2+crad+Curcubelen*2);
        glRotatef(90,0,0,1);
        glRotatef(180,0,0,1);
        glRotatef(90,1,0,0);
        glRotatef(90,0,1,0);
        drawCylinderPart(crad,2*ch,20);
    }glPopMatrix();

    glPushMatrix();{
        glTranslatef(-Curcubelen,-Curcubelen,2+crad+Curcubelen*2);
        glRotatef(90,0,0,1);
         glRotatef(90,1,0,0);
        glRotatef(90,0,1,0);
        drawCylinderPart(crad,2*ch,20);
    }glPopMatrix();

    //sphere

    glPushMatrix();{            //sp 1 (right upper)
        glColor3f(1,0,0);
        glTranslatef(-Curcubelen,Curcubelen,2+crad+2*Curcubelen);
        glRotatef(90,0,0,1);
        drawSpherePart(crad,20,20);
    }glPopMatrix();

     glPushMatrix();{            //sp 2 (left upper)
        glColor3f(1,0,0);
        glTranslatef(Curcubelen,Curcubelen,2+crad+2*Curcubelen);
        drawSpherePart(crad,20,20);
    }glPopMatrix();

     glPushMatrix();{            //frm sp 1 (pichone right upper)
        glColor3f(1,0,0);
        glTranslatef(-Curcubelen,-Curcubelen,2+crad+2*Curcubelen);
        glRotatef(90,0,0,1);
        glRotatef(90,0,0,1);
        drawSpherePart(crad,20,20);
    }glPopMatrix();

     glPushMatrix();{            //frm sp 2 (pichone left upper)
        glColor3f(1,0,0);
        glTranslatef(Curcubelen,-Curcubelen,2+crad+2*Curcubelen);
        glRotatef(90,0,1,0);
        glRotatef(-180,0,0,1);
        drawSpherePart(crad,20,20);
    }glPopMatrix();

        ///lower portion

    glPushMatrix();{            //sp 1 (right niche piche)
        glColor3f(1,0,0);
        glTranslatef(-Curcubelen,-Curcubelen,2+crad);
        glRotatef(180,1,0,0);
        glRotatef(90,0,0,1);
        drawSpherePart(crad,20,20);
    }glPopMatrix();

    glPushMatrix();{            // (right niche piche)
        glColor3f(1,0,0);
        glTranslatef(-Curcubelen,-Curcubelen,2+crad);
        glRotatef(180,1,0,0);
        glRotatef(90,0,0,1);
        drawSpherePart(crad,20,20);
    }glPopMatrix();

     glPushMatrix();{            // (right niche samne)
        glColor3f(1,0,0);
        glTranslatef(-Curcubelen,Curcubelen,2+crad);
        glRotatef(90,1,0,0);
        glRotatef(180,1,0,0);
        glRotatef(90,0,0,1);
        drawSpherePart(crad,20,20);
    }glPopMatrix();

    glPushMatrix();{            // (left niche samne)
        glColor3f(1,0,0);
        glTranslatef(Curcubelen,Curcubelen,2+crad);
        glRotatef(90,1,0,0);
        glRotatef(180,1,0,0);
        //glRotatef(90,0,0,1);
        drawSpherePart(crad,20,20);
    }glPopMatrix();

     glPushMatrix();{            // (right niche piche)
        glColor3f(1,0,0);
        glTranslatef(Curcubelen,-Curcubelen,2+crad);
        glRotatef(180,1,0,0);
        //glRotatef(90,0,0,1);
        drawSpherePart(crad,20,20);
    }glPopMatrix();





}


void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(50,50,100,	0,0,0,	0,1,0);

	//for camera
	gluLookAt(pos.x, pos.y, pos.z, pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawCube();

	//drawgrid=1;
	//drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();

//    drawCircle(30,80);          //wheel


    //drawCone(20,50,24);

	//drawSpherePart(30,24,20);           //radius, slices, stack;

    //glutSolidTorus(20,50,5,3);

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);

	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance

	//for camera
	pos.x=100; pos.y=100, pos.z=0;
	u.x=0; u.y=0; u.z=1;
	r.x= -1/sqrt(2); r.y= 1/sqrt(2); r.z= 0;
	l.x=-1/sqrt(2); l.y=-1/sqrt(2); l.z=0;
	//printf("%lf ", l.x*r.x+l.y*r.y+l.z*r.z);


}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
