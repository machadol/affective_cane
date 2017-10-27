import toxi.geom.*;
import toxi.geom.mesh.*;

import toxi.processing.*;

TriangleMesh mesh;
ToxiclibsSupport gfx;

void setup() {
  size(600,600,P3D);
  //mesh=(TriangleMesh)new STLReader().loadBinary(sketchPath("BENGA.stl"),STLReader.TRIANGLEMESH);
  mesh=(TriangleMesh)new STLReader().loadBinary(sketchPath("BENGA.stl"),STLReader.TRIANGLEMESH).flipYAxis();
  gfx=new ToxiclibsSupport(this);
}

void draw() {
  background(51);
  lights();
  translate(mouseX,mouseY,-500);
  //rotateX(mouseY*0.01);
  //rotateY(mouseX*0.01);
  gfx.origin(new Vec3D(),200);
  noStroke();
  gfx.mesh(mesh);
  

}
