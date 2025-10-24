#include <cmath>
#include <numbers>

#include <osg/Geode>
#include <osg/StateSet>
#include <osg/Texture2D>

#include <osgDB/ReadFile>

#include <osgViewer/Viewer>

#include <osgGA/TrackballManipulator>

static const std::string k_asset_dir = ASSET_DIR;

osg::Geometry* createSphere(float radius, unsigned char sector_count, unsigned char stack_count) {
  osg::Geometry* geometry   = new osg::Geometry;
  osg::Vec3Array* normals   = new osg::Vec3Array;
  osg::Vec3Array* vertices  = new osg::Vec3Array;
  osg::Vec2Array* texcoords = new osg::Vec2Array;

  unsigned int k1 = 0, k2 = 0;
  
  float x = 0.0f, y = 0.0f, z = 0.0f, xz = 0.0f, u = 0.0f, v = 0.0f;
  float sector_angle = 0.0f, stack_angle = 0.0f;

  float u_step = 1.0f / sector_count;
  float v_step = 1.0f / stack_count;
  float stack_step = std::numbers::pi / stack_count;
  float sector_step = 2.0f * std::numbers::pi / sector_count;

  std::vector<osg::Vec3> tmp_vertices;
  std::vector<osg::Vec3> tmp_normals;
  std::vector<osg::Vec2> tmp_texcoords;
  std::vector<unsigned int> indices;

  for(int i = 0; i < stack_count + 1; ++i) {
      stack_angle = std::numbers::pi / 2.0f - (i * stack_step);
 
      k1 = i * (sector_count + 1);
      k2 = k1 + sector_count + 1;

      v = i * v_step;
      y = sinf(stack_angle);
      xz = cosf(stack_angle);

    for(int j = 0; j < sector_count + 1; ++j, ++k1, ++k2) {
      sector_angle= j * sector_step;
      u = j * u_step;

      osg::Vec3 normal(xz * cosf(sector_angle), y, xz * sinf(sector_angle));

      tmp_normals.push_back(normal);
      tmp_vertices.push_back(normal * radius);
      tmp_texcoords.push_back(osg::Vec2(u, v));

      if (i < stack_count && j < sector_count) {
        indices.push_back(k1);
        indices.push_back(k2);
        indices.push_back(k1 + 1);

        indices.push_back(k1 + 1);
        indices.push_back(k2);
        indices.push_back(k2 + 1);
      }
    }
  }

  for (int i = 0; i < indices.size(); i += 3) { // tbh I don't know any better way in OSG
    vertices->push_back(tmp_vertices[indices[i]]);
    vertices->push_back(tmp_vertices[indices[i + 1]]);
    vertices->push_back(tmp_vertices[indices[i + 2]]);

    normals->push_back(tmp_normals[indices[i]]);
    normals->push_back(tmp_normals[indices[i + 1]]);
    normals->push_back(tmp_normals[indices[i + 2]]);

    texcoords->push_back(tmp_texcoords[indices[i]]);
    texcoords->push_back(tmp_texcoords[indices[i + 1]]);
    texcoords->push_back(tmp_texcoords[indices[i + 2]]);
  }

  geometry->setNormalArray(normals);
  geometry->setVertexArray(vertices);
  geometry->setTexCoordArray(0, texcoords);

  geometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
  geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, vertices->size()));

  return geometry;
}

int main(void) {
  osg::ref_ptr<osg::Vec4Array> color_array = new osg::Vec4Array;
  color_array->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));

  osg::ref_ptr<osg::Geometry> sphere_geometry = createSphere(1.0f, 64, 32);
  sphere_geometry->setColorArray(color_array.get(), osg::Array::BIND_OVERALL);

  osg::ref_ptr<osg::Geode> geode = new osg::Geode;
  geode->addDrawable(sphere_geometry.get());

  osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D(osgDB::readImageFile(k_asset_dir + "/red_brick_diff_4k.jpg"));
  geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture);

  osg::ref_ptr<osg::Group> root = new osg::Group;
  root->addChild(geode.get());

  osgViewer::Viewer viewer;
  viewer.setSceneData(root);
  viewer.setCameraManipulator(new osgGA::TrackballManipulator());
  viewer.setUpViewInWindow(100, 100, 800, 600);

  return viewer.run();
}