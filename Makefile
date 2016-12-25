# The authors of this work have released all rights to it and placed it
# in the public domain under the Creative Commons CC0 1.0 waiver
# (http://creativecommons.org/publicdomain/zero/1.0/).
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# 
# Retrieved from: http://en.literateprograms.org/Hello_World_(C,_Cairo)?oldid=10388

CFLAGS=-Wall -pedantic -std=c11 -g -I/usr/X11R6/include `pkg-config --cflags cairo`
LDFLAGS=-Wall -g `pkg-config --libs cairo` -lm  -lX11 -L/usr/X11R6/lib

all: graph

graph: main.o matrix.o matrix_gen.o
	cc -o graph main.o matrix.o matrix_gen.o ${LDFLAGS}

main.o: main.c
	cc -o main.o ${CFLAGS} -c main.c
matrix.o: matrix.c
	cc -o matrix.o ${CFLAGS} -c matrix.c
matrix_gen.o: matrix.c
	cc -o matrix_gen.o ${CFLAGS} -c matrix_gen.c
clean:
	rm *.o *.png out.mp4
video:
	ffmpeg -framerate 60 -i IMG%03d.png -c:v libx264 -y -r 30 -pix_fmt yuv420p out.mp4
