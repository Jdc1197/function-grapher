/* The authors of this work have released all rights to it and placed it
in the public domain under the Creative Commons CC0 1.0 waiver
(http://creativecommons.org/publicdomain/zero/1.0/).

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
ERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Retrieved from: http://en.literateprograms.org/Hello_World_(C,_Cairo)?oldid=10388
*/


#include<cairo.h>
#include<cairo-xlib.h>
#include<X11/Xlib.h>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include <math.h>

#include "matrix.h"

/* Screen Size */
#define SIZEX 500
#define SIZEY 500

/* Graph Size*/
#define XMIN -12
#define XMAX 12
#define YMIN -12
#define YMAX 12
#define GSIZEX (XMAX-XMIN)
#define GRATIOX (SIZEX/GSIZEX) /* GRATIOX is the ratio of screen size to graph size.*/
                               /* you can use it to convert graph coordinates to actual screen coordinates.*/
#define GSIZEY (YMAX-YMIN)
#define GRATIOY (SIZEY/GSIZEY)

#define GORGINX (SIZEX/2)+((XMAX+XMIN)/2)*GRATIOX
#define GORGINY (SIZEY/2)+((YMAX+YMIN)/2)*GRATIOY

/* Steps */
#define STEPS 50

const double pi = 3.14159265L;
void draw_function(cairo_t*, double);
void draw_matrix(cairo_t*, matrix_t*, double, double, double, double);

void paint(cairo_surface_t *cs, double st)
{
	cairo_t *c;
	c=cairo_create(cs);
	/* Draw BG */
    cairo_set_source_rgb(c, 1.0, 1.0, 1.0);
    cairo_rectangle(c, 0.0, 0.0, SIZEX, SIZEY);
    cairo_fill(c);
    
    draw_function(c, st);
	
    cairo_show_page(c);
cairo_destroy(c);
}

void draw_function(cairo_t* c, double st)
{
    matrix_t m = create_matrix(STEPS);
    for (int i = 0; i < STEPS; i++)
    {
        m.x[i] = (5+i*2*pi/STEPS);
        m.y[i] = -(sin(m.x[i]-5));
        m.z[i] = 0;
    }
    pmatrix_t pm = convert_to_pmatrix(m, 0, 0, 0);
    shift_pmatrix(&pm,0,st);
    matrix_t m2 = convert_to_matrix(pm);
    draw_matrix(c, &m2, .5, .5, .5, 2.0);
    free_matrix(&m2);
    free_pmatrix(&pm);
    
    //shift_pmatrix(&pm, 0, 0);
    //m = convert_to_matrix(pm);
    draw_matrix(c, &m, 0.0, 0.0, 0.0, 2.0);
    free_matrix(&m);
}

/* Draws a matrix 'm' onto cairo instance 'c' with size 's' and color 'r,g,b'*/
void draw_matrix(cairo_t* c, matrix_t* m, double r, double g, double b, double s)
{
    
    double ydisplacement = GORGINY;
    double xdisplacement = GORGINX;

    cairo_move_to(c, (m->x[0]*GRATIOX)+xdisplacement, (m->y[0]*GRATIOY)+ydisplacement);
    for ( int i = 0; i < m->size; i++)
    {
        cairo_line_to(c, (m->x[i]*GRATIOX)+xdisplacement,(m->y[i]*GRATIOY)+(ydisplacement));
    }
    cairo_set_source_rgb(c, r, g, b);
    cairo_set_line_width(c, s);
    cairo_stroke(c);
}

void showxlib()
{
	Display *dpy;
	Window rootwin;
	Window win;
	XEvent e;
	int scr;
	cairo_surface_t *cs;

	if(!(dpy=XOpenDisplay(NULL))) {
		fprintf(stderr, "ERROR: Could not open display\n");
		exit(1);
	}

	scr=DefaultScreen(dpy);
	rootwin=RootWindow(dpy, scr);

	win=XCreateSimpleWindow(dpy, rootwin, 1, 1, SIZEX, SIZEY, 0, 
			BlackPixel(dpy, scr), BlackPixel(dpy, scr));

	XStoreName(dpy, win, "hello");
	XSelectInput(dpy, win, ExposureMask|ButtonPressMask);
	XMapWindow(dpy, win);

	cs=cairo_xlib_surface_create(dpy, win, DefaultVisual(dpy, 0), SIZEX, SIZEY);

	while(1) {
		XNextEvent(dpy, &e);
		if(e.type==Expose && e.xexpose.count<1) {
			paint(cs, pi/12);
		} else if(e.type==ButtonPress) break;
	}

	cairo_surface_destroy(cs);
	XCloseDisplay(dpy);
}

void writepng(const char *fname, int steps)
{
	for (int i = 0; i <= steps; i++)
    {
    cairo_surface_t *cs;
    cs=cairo_image_surface_create(CAIRO_FORMAT_ARGB32, SIZEX, SIZEY);
    
	printf("%f\n", (2*pi)*((double)(i)/(double)(steps)));
    paint(cs, ((2*pi)*((double)(i)/(double)(steps))));
	char fn[11] = "IMG000.png\0";
    fn[5] = (char)((i%10)+48);
    fn[4] = (char)(((int)(i/10)%10)+48); 
    fn[3] = (char)(((int)(i/100)%10)+48);
    cairo_surface_write_to_png(cs, fn);
	cairo_surface_destroy(cs);
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    //writepng("",600);
    showxlib();
	return 0;
}

