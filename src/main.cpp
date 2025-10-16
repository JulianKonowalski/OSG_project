#include <osg/Geode>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osgGA/TrackballManipulator>

int main(int argc, char** argv) {
  osg::ref_ptr<osg::Box> box = new osg::Box(osg::Vec3(0, 0, 0), 1.0f);
  osg::ref_ptr<osg::ShapeDrawable> boxDrawable = new osg::ShapeDrawable(box);
  boxDrawable->setColor(osg::Vec4(0.2f, 0.7f, 1.0f, 1.0f));

  osg::ref_ptr<osg::Geode> geode = new osg::Geode();
  geode->addDrawable(boxDrawable);

  osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform();
  transform->addChild(geode);

  osgViewer::Viewer viewer;
  viewer.setSceneData(transform);
  viewer.setCameraManipulator(new osgGA::TrackballManipulator());
  viewer.setUpViewInWindow(100, 100, 800, 600);

  double angle = 0.0;
  while (!viewer.done()) {
    angle += 0.5;
    transform->setMatrix(osg::Matrix::rotate(
        osg::DegreesToRadians(angle), 
        osg::Vec3(0.0f, 0.0f, 1.0f)
    ));
    viewer.frame();
  }

  return 0;
}