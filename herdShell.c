// 
// herd
// 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
// 
#define Z 800
#define N 100
#define DT 0.01 
#define R 0.010
// 
typedef struct {
    double x ;
    double y ;
    //
    double v ;
    double t ;
    //
    double dv ;
    double dt ;
    //
} Node; 

typedef struct {
    double x;
    double y;
    double r;
} Obstacle;
// 
Node arr[N] ; 
#define O 8
Obstacle obs[O] = {
    {
        .x = -1.6,
        .y = 0.5,
        .r = 2,
    },
    {
        .x = 2.6,
        .y = 0.5,
        .r = 2,
    },
    {
        .x = 0.3,
        .y = 0.5,
        .r = 0.17,
    },
    {
        .x = 0.7,
        .y = 0.5,
        .r = 0.17,
    },
};
// 
int  nbr[N] ; 
// 
double myrand()
{
    return ( 1.0 * rand() ) / RAND_MAX ;
} 
// 
double mydist( double x1 , double y1 , double x2 , double y2 )
{
    double dx = x2 - x1 ;
    double dy = y2 - y1 ;
    //
    return sqrt( dx*dx + dy*dy ) ;
} 
// 
void idlefunc(void)
{
    int j , k , kmin ;
    //
    double d , dmin ;
    //
    for( j = 0 ; j < N ; j++ )
    {
        arr[j].x += ( arr[j].v * cos( arr[j].t ) * DT ) ;
        arr[j].y += ( arr[j].v * sin( arr[j].t ) * DT ) ;
        //
        // bounce
        //
        if( arr[j].y - R < 0.0 )
        {
            arr[j].y = R ;
            //
            arr[j].t *= -1.0        ;
            arr[j].t +=  2.0 * M_PI ;
        }
        else if( arr[j].y + R > 1.0 )
        {
            arr[j].y = 1.0 - R ;
            //
            arr[j].t *= -1.0        ;
            arr[j].t +=  2.0 * M_PI ;
        }
        //
        if( arr[j].x - R < 0.0 )
        {
            arr[j].x = R ;
            //
            arr[j].t *= -1.0        ;
            arr[j].t +=  1.0 * M_PI ;
        }
        else if( arr[j].x + R > 1.0 )
        {
            arr[j].x = 1.0 - R ;
            //
            arr[j].t *= -1.0        ;
            arr[j].t +=  1.0 * M_PI ;
        }
    }
    //
    //
    //
    for( j = 0 ; j < N ; j++ )
    {
        kmin = j ;
        //
        for( k = 0 ; k < N ; k++ )
        {
            if( k != j )
            {
                d = mydist( arr[j].x , arr[j].y , arr[k].x , arr[k].y ) ;
                //
                if( kmin == j || d < dmin )
                {
                    kmin = k ;
                    dmin = d ;
                }
            }
        }
        //
        nbr[j] = kmin ;
    }
    //
    //
    //
    float changeFactor = 0.1;
    float influenceFactor = 0.7;
    for( j = 0 ; j < N ; j++ ) {
        k = nbr[j];
        //
        double dv = arr[k].v - arr[j].v;
        double dt = arr[k].t - arr[j].t;
        if (dt < -M_PI) {
            dt += 2*M_PI;
        } else if (dt > M_PI) {
            dt -= 2*M_PI;
        }

        arr[j].dv = changeFactor * (dv * influenceFactor * myrand() - dv * (1-influenceFactor));
        arr[j].dt = changeFactor * (dt * influenceFactor * myrand() - dt * (1-influenceFactor));

        double dist = mydist(arr[k].x, arr[k].y, arr[j].x, arr[j].y);

        if (dist < 3*R) {
            double dx = arr[k].x - arr[j].x;
            double dy = arr[k].y - arr[j].y;
            
            dist = 3*R-dist;

            double forceAngle = atan2(-dy, -dx);
            double forceMag = dist * dist;
            
            arr[j].dv += forceMag * 100 * cos(forceAngle - arr[j].t);
            double forceDt = forceAngle - arr[j].t;
            if (forceDt < -M_PI) forceDt += 2*M_PI;
            else if (forceDt > M_PI) forceDt -= 2*M_PI;
            arr[j].dt += changeFactor * forceDt * 0.3;
        }

        for (k=0;k<O;++k) {
            double dist = mydist(arr[j].x, arr[j].y, obs[k].x, obs[k].y);

            if (dist < 2*R + obs[k].r) {
                double dx = obs[k].x - arr[j].x;
                double dy = obs[k].y - arr[j].y;
                
                dist = 2*R+obs[k].r-dist;

                double forceAngle = atan2(-dy, -dx);
                double forceMag = dist * dist;
                
                arr[j].dv += forceMag * 1000 * cos(forceAngle - arr[j].t);
                double forceDt = forceAngle - arr[j].t;
                if (forceDt < -M_PI) forceDt += 2*M_PI;
                else if (forceDt > M_PI) forceDt -= 2*M_PI;
                arr[j].dt += changeFactor * forceDt * 0.8;
                break;
            }
        }
    }
    //
    for( j = 0 ; j < N ; j++ )
    {
        arr[j].v += arr[j].dv ;
        if( arr[j].v > 0.15 ) arr[j].v = 0.15 ;
        if( arr[j].v < 0.05 ) arr[j].v = 0.05 ;
        //
        arr[j].t += arr[j].dt ;
        if( arr[j].t > 2.0 * M_PI ) arr[j].t -= ( 2.0 * M_PI ) ;
        if( arr[j].t < 0.0        ) arr[j].t += ( 2.0 * M_PI ) ;
    }
    //
    glutPostRedisplay() ;
}
//
void displayfunc(void)
{
    int j , k ;
    //
    double x  , y  ;
    //
    double x1 , y1 ;
    double x2 , y2 ;
    double x3 , y3 ;
    //
    double t  , dt = 0.01 ;
    //
    glClear(GL_COLOR_BUFFER_BIT);
    //
    //
    //
    glColor3f( 1.0 , 1.0 , 0.0 ) ; // nbr ... edge
    //
    glBegin( GL_LINES ) ;
    //
    for( j = 0 ; j < N ; j++ )
    {
        k = nbr[j] ;
        //
        glVertex2f( arr[j].x , arr[j].y ) ;
        glVertex2f( arr[k].x , arr[k].y ) ;
    }
    //
    glEnd() ;
    //
    //
    for (j=0;j<O;++j) {
        glColor3f(0.7, 0.7, 0.7);
        t = 0;
        while (t < 2 * M_PI) {
            x1 = obs[j].x                     ;
            y1 = obs[j].y                     ;
            //
            x2 = obs[j].x + obs[j].r * cos( t )      ;
            y2 = obs[j].y + obs[j].r * sin( t )      ;
            //
            x3 = obs[j].x + obs[j].r * cos( t + dt ) ;
            y3 = obs[j].y + obs[j].r * sin( t + dt ) ;
            //
            glBegin( GL_TRIANGLES ) ;
            glVertex2f( x1 , y1 ) ;
            glVertex2f( x2 , y2 ) ;
            glVertex2f( x3 , y3 ) ;
            glEnd() ;
            //
            t += dt ;
        }
    }
    //
    for( j = 0 ; j < N ; j++ )
    {
        glColor3f( 1.0 , 1.0 , 1.0 ) ; // circle
        //
        t = 0.0 ;
        while( t < 2.0 * M_PI )
        {
            x1 = arr[j].x                     ;
            y1 = arr[j].y                     ;
            //
            x2 = arr[j].x + R * cos( t )      ;
            y2 = arr[j].y + R * sin( t )      ;
            //
            x3 = arr[j].x + R * cos( t + dt ) ;
            y3 = arr[j].y + R * sin( t + dt ) ;
            //
            glBegin( GL_TRIANGLES ) ;
            glVertex2f( x1 , y1 ) ;
            glVertex2f( x2 , y2 ) ;
            glVertex2f( x3 , y3 ) ;
            glEnd() ;
            //
            t += dt ;
        }
        //
        //
        //
        glColor3f( 1.0 , 0.0 , 0.0 ) ; // red arrow
        //
        x = arr[j].x ;
        y = arr[j].y ;
        t = arr[j].t ;
        //
        x1 = x + R * cos( t + 0.5 * M_PI ) ;
        y1 = y + R * sin( t + 0.5 * M_PI ) ;
        //
        x2 = x + R * cos( t              ) ;
        y2 = y + R * sin( t              ) ;
        //
        x3 = x + R * cos( t - 0.5 * M_PI ) ;
        y3 = y + R * sin( t - 0.5 * M_PI ) ;
        //
        glBegin(GL_TRIANGLES);
            glVertex2f( x1 , y1 );
            glVertex2f( x2 , y2 );
            glVertex2f( x3 , y3 );
        glEnd();
        //
        x1 = x + 0.4 * R * cos( t - 0.5 * M_PI )                             ;
        y1 = y + 0.4 * R * sin( t - 0.5 * M_PI )                             ;
        //
        x2 = x + 0.4 * R * cos( t + 0.5 * M_PI ) + 0.8 * R * cos( t + M_PI ) ;
        y2 = y + 0.4 * R * sin( t + 0.5 * M_PI ) + 0.8 * R * sin( t + M_PI ) ;
        //
        x3 = x + 0.4 * R * cos( t + 0.5 * M_PI )                             ;
        y3 = y + 0.4 * R * sin( t + 0.5 * M_PI )                             ;
        //
        glBegin(GL_TRIANGLES);
            glVertex2f( x1 , y1 );
            glVertex2f( x2 , y2 );
            glVertex2f( x3 , y3 );
        glEnd();
        //
        x3 = x + 0.4 * R * cos( t - 0.5 * M_PI ) + 0.8 * R * cos( t + M_PI ) ;
        y3 = y + 0.4 * R * sin( t - 0.5 * M_PI ) + 0.8 * R * sin( t + M_PI ) ;
        //
        glBegin(GL_TRIANGLES);
            glVertex2f( x1 , y1 );
            glVertex2f( x2 , y2 );
            glVertex2f( x3 , y3 );
        glEnd();
    }
    //
    //
    //
    glutSwapBuffers() ;
}
int main(int argc,char* argv[])
{  
    int j ;
    //
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(Z,Z);
    glutInitWindowPosition(100,50);
    glutCreateWindow("");
    glClearColor(0.0,0.0,0.0,0.0);
    glShadeModel(GL_SMOOTH);
    //
    glViewport(0,0,(GLsizei)Z,(GLsizei)Z); // reshape
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,1.0,0.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    //
    srand(time(NULL));
    for( j = 0 ; j < N ; j++ )
    {
        nbr[j] = j ; // no nbr ... default motion
        //
        arr[j].x = myrand() ;
        arr[j].y = myrand() ;
        //
        arr[j].v = myrand() * 0.1 + 0.05 ;
        arr[j].t = myrand() * 2.0 * M_PI ;
    }

//    for (j=0;j<O;++j) {
//       obs[j].x = myrand();
//       obs[j].y = myrand();
//       obs[j].r = 0.05 + myrand() * 0.15;
//    }
    //
    glutIdleFunc(idlefunc);
    glutDisplayFunc(displayfunc);
    //
    glutMainLoop();
    //
    return 0;
}
// 
// end of file
// 
