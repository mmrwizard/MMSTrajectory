#include <osg/Geode>
#include <osg/Geometry>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/DisplaySettings>
#include <osg/LightModel>
#include <osg/LineWidth>

#include <fstream>
#include <sstream>

osg::ref_ptr<osg::Vec3Array> readPosFile(const std::string& filename)
{
	// Create a new Vec3Array to hold the vertices
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;

	// Open the file
	std::ifstream inFile(filename);
	if (!inFile)
	{
		std::cerr << "Unable to open " << filename << "\n";
		return vertices;  // return empty vertices array
	}

	// Read the file line by line
	std::string line;
	while (std::getline(inFile, line))
	{
		std::istringstream ss(line);
		float val1, val2, val3, val4;
		ss >> val1 >> val2 >> val3 >> val4; // assuming four columns of floats

		vertices->push_back(osg::Vec3(val2, val3, val4)); // Create a vertex from the XYZ coordinates
	}

	return vertices;
}

int main()
{
	// Create a LineWidth object and set its width
	osg::ref_ptr<osg::LineWidth> linewidth = new osg::LineWidth;
	linewidth->setWidth(5.0f);

	// Create a red color
	osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array;
	color->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));

	// Create a geometry node for first line
	osg::ref_ptr<osg::Geometry> geom1 = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> vertices1 = readPosFile("poslessG.txt");
	geom1->setColorArray(color.get());
	geom1->setColorBinding(osg::Geometry::BIND_OVERALL);
	geom1->setVertexArray(vertices1.get());
	geom1->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, vertices1->size()));
	geom1->getOrCreateStateSet()->setAttributeAndModes(linewidth, osg::StateAttribute::ON);

	osg::ref_ptr<osg::Vec4Array> color2 = new osg::Vec4Array;
	color2->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));  // Set to green color

	// Create a geometry node for second line
	osg::ref_ptr<osg::Geometry> geom2 = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> vertices2 = readPosFile("pos.txt");
	geom2->setColorArray(color2.get());  // Apply the new color
	geom2->setColorBinding(osg::Geometry::BIND_OVERALL);
	geom2->setVertexArray(vertices2.get());
	geom2->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, vertices2->size()));
	geom2->getOrCreateStateSet()->setAttributeAndModes(linewidth, osg::StateAttribute::ON);


	// Create a geode and add the geometry nodes to it
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(geom1.get());
	geode->addDrawable(geom2.get());

	// Load the local model
	osg::ref_ptr<osg::Node> localModel = osgDB::readNodeFile("G:\\imagedata\\swjtu_uav_data\\4Model\\XP\\xnjd-osgb\\Production_1\\root.osgb");
	if (!localModel)
	{
		std::cerr << "Unable to read model.s3c";
		return 1;
	}

	// Add both the lines and the local model to a group node
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(geode.get());
	root->addChild(localModel.get());

	// Disable lighting
	root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	// Create the viewer
	osgViewer::Viewer viewer;
	viewer.setSceneData(root.get());

	// Set the background color to white
	viewer.getCamera()->setClearColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));

	return viewer.run();
}
