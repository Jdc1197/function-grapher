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

#define GORGINX (SIZEX/2)+((XMAX+XMIN)/-2)*GRATIOX
#define GORGINY (SIZEY/2)+((YMAX+YMIN)/2)*GRATIOY

/* Steps */
#define STEPS 50

const double pi = 3.14159265L;

matrix_t generate_matrix(double (*f)(double),double, double, int);
double s(double x) {return cos(x);}

void draw_matrix(cairo_t*, matrix_t*, double, double, double, double);

void draw_grid(cairo_t *cr)
{
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); 
    cairo_select_font_face(cr, "Arial",
    CAIRO_FONT_SLANT_NORMAL,
    CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 13);

    cairo_move_to(cr, 0, 10);
    cairo_show_text(cr, "f(x) = cosx");  
    /* Vertical gridline */
    cairo_move_to(cr, GORGINX,0);
    cairo_line_to(cr, GORGINX, SIZEY);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 2.0);   
    cairo_stroke(cr); 
    
    /* Horizontal gridline */
    cairo_move_to(cr, 0, GORGINY);
    cairo_line_to(cr, SIZEX, GORGINY);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 2.0);   
    cairo_stroke(cr);  
}

void paint(cairo_surface_t *cs, double st)
{
	cairo_t *c;
	c=cairo_create(cs);
	/* Draw BG */
    cairo_set_source_rgb(c, 1.0, 1.0, 1.0);
    cairo_rectangle(c, 0.0, 0.0, SIZEX, SIZEY);
    cairo_fill(c);
    
    matrix_t m = generate_matrix(s, -2*pi, 2*pi, 100);
    draw_matrix(c,&m,.5,0,0,1.5);
    draw_grid(c);
    cairo_show_page(c);
    cairo_destroy(c);
}

/* Generates a matrix from a function*/
matrix_t generate_matrix(   double (*f)(double),    /* function to translate x->y*/
                            double min_x,           /* min and max x */ 
                            double max_x,
                            int steps)              /* matrix size*/
{
    
    matrix_t m = create_matrix(steps);
    for (int i = 0; i < steps; i++)
    {
        m.x[i] = min_x+((max_x-min_x)*((double)i/(steps-1)));
        m.y[i] = f((m.x[i]));
    } 
    return m;
}

/* Draws a matrix 'm' onto cairo instance 'c' with size 's' and color 'r,g,b'*/
void draw_matrix(cairo_t* c, matrix_t* m, double r, double g, double b, double s)
{
    double ydisplacement = GORGINY;
    double xdisplacement = GORGINX;
    cairo_move_to(c, (m->x[0]*GRATIOX)+xdisplacement, (-m->y[0]*GRATIOY)+ydisplacement);
    for ( int i = 0; i < m->size; i++)
    {
        cairo_line_to(c, (m->x[i]*GRATIOX)+xdisplacement,(-m->y[i]*GRATIOY)+(ydisplacement));
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
			paint(cs, 0);
		} else if(e.type==ButtonPress) break;
	}

	cairo_surface_destroy(cs);
	XCloseDisplay(dpy);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    //matrix_t m = generate_matrix(s, -2*pi, 2*pi, 100);
    //render_rotation_animation(m,2*pi,pi,300);    
    showxlib();
	return 0;
}

