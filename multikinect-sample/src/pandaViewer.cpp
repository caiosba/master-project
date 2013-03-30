/*
 *  pandaViewer.cpp
 *
 *  Created on: Mar 25, 2013
 *  Author: heekinho
 */

#include "pandaViewer.h"

#include "pandaFramework.h"
#include "load_prc_file.h"
#include "texture.h"
#include "pta_uchar.h"
#include "pta_ushort.h"
#include "cardMaker.h"
#include "nodePath.h"
#include "geomVertexData.h"
#include "geomTriangles.h"
#include "geomPoints.h"
#include "shaderPool.h"
#include "pnmImage.h"
#include "transparencyAttrib.h"
#include "omniBoundingVolume.h"

PandaFramework* pframework;
WindowFramework* pwindow;

NodePath render, scene;
int width;
int height;


NodePath camera[2] = { NodePath("cam1"), NodePath("cam2") };
NodePath frames[2] = { NodePath("frame1"), NodePath("frame2") };
Texture* texture_rgb[2];
Texture* texture_depth[2];
NodePath helper[2] = { NodePath("frame1"), NodePath("frame2") };


void set_mat(Mat m, int frame){
	if(frame >= 2) return;
	LMatrix4f pmat = LMatrix4f(
			m.at<double>(0,0), m.at<double>(0,1), m.at<double>(0,2), m.at<double>(0,3),
			m.at<double>(1,0), m.at<double>(1,1), m.at<double>(1,2), m.at<double>(1,3),
			m.at<double>(2,0), m.at<double>(2,1), m.at<double>(2,2), m.at<double>(2,3),
			m.at<double>(3,0), m.at<double>(3,1), m.at<double>(3,2), m.at<double>(3,3));
	pmat.transpose_in_place();

	camera[frame].set_mat(pmat);

	LVecBase3 pos = camera[frame].get_pos();
	LVecBase3 hpr = camera[frame].get_hpr();

	camera[frame].set_pos(0, 0, 0);
	camera[frame].set_hpr(0, 0, 0);

	camera[frame].set_x(-pos[1] * (0.01 * 2.3f) );
	camera[frame].set_y(-pos[0] * (0.01 * 2.3f) );
	camera[frame].set_z(-pos[2]  * (0.01 * 2.3f) );

	camera[frame].set_p(/*camera[frame], */ -hpr[1]);
	camera[frame].set_r(/*camera[frame], */180+hpr[2]);
	camera[frame].set_h(/*camera[frame], */90-hpr[0]);
}



NodePath create_point_cloud(int width, int height){
	PT(GeomVertexData) vdata = new GeomVertexData("name", GeomVertexFormat::get_v3n3(), Geom::UH_static);

	GeomVertexWriter vertex, normal, color, texcoord;
	vertex = GeomVertexWriter(vdata, "vertex");

	// Creates a grid with z = 0 for all points. The shader will do the work
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			vertex.add_data3f(i, j, 0.0f);
		}
	}

	PT(GeomPoints) prim = new GeomPoints(Geom::UH_static);
	for(int i = 0; i < width * height; i++) prim->add_vertex(i);
	prim->close_primitive();

//	// Close the points
//	PT(GeomTriangles) prim = new GeomTriangles(Geom::UH_static);
//	for(int i = 0; i < width-1; i++){
//		for(int j = 0; j < height-1; j++){
//			int ref = i*height + j;
//			int a = ref;
//			int b = ref+height;
//			int c = a+1;
//			int d = b+1;
//			prim->add_vertices(a, b, c);
//			prim->add_vertices(c, b, d);
//		}
//	}
//	prim->close_primitive();


	PT(Geom) cloud_geom = new Geom(vdata);
	cloud_geom->add_primitive(prim);

	PT(GeomNode) cloud_gn = new GeomNode("depthmap");
	cloud_gn->add_geom(cloud_geom);

	NodePath cloud_np = NodePath(cloud_gn);
	cloud_np.set_render_mode_thickness(2);

	return cloud_np;
}


void set_rgb_data(int width, int height, void* data, int frame){
	if(frame >= 2) return;

	texture_rgb[frame]->set_compression(Texture::CM_off);
	texture_rgb[frame]->setup_2d_texture(width, height, Texture::T_unsigned_byte, Texture::F_rgb);
	texture_rgb[frame]->set_keep_ram_image(true);

	int size = width * height * 3;
	PTA_uchar p = PTA_uchar::empty_array(sizeof(uchar) * size);

	// Copy directly since the set_data seems to be buggy
	memcpy(p.p(), data, sizeof(uchar) * size);
	texture_rgb[frame]->set_ram_image(CPTA_uchar(p));
}

void set_depth_data(int width, int height, void* data, int frame){
	if(frame >= 2) return;

	texture_depth[frame]->set_compression(Texture::CM_off);
	texture_depth[frame]->setup_2d_texture(width, height, Texture::T_unsigned_short, Texture::F_depth_component);
	texture_depth[frame]->set_keep_ram_image(true);

	int size = width * height * 1;
	PTA_uchar p = PTA_uchar::empty_array(sizeof(ushort) * size);

	// Copy directly since the set_data seems to be buggy
	memcpy(p.p(), data, sizeof(ushort) * size);
	texture_depth[frame]->set_ram_image(CPTA_uchar(p));
}



void create_frame(int f){
	frames[f] = create_point_cloud(width, height);
	frames[f].reparent_to(scene);
	helper[f].reparent_to(scene);
	frames[f].set_shader(ShaderPool::load_shader("depth.cg"));
//	frames[f].set_scale(0.1);
//	frames[f].set_p(90);
//	frames[f].set_pos(-width/20, 0, -height/20);
//	frames[f].flatten_light();
//	frames[f].set_two_sided(true);
	frames[f].set_transparency(TransparencyAttrib::M_alpha);

	frames[f].set_texture(new TextureStage("depth"), texture_depth[f]);
	frames[f].set_texture(new TextureStage("color"), texture_rgb[f]);

	frames[f].set_shader_input("mycam", camera[f]);
	frames[f].set_shader_input("mycam_pos", camera[f].get_pos());

	helper[f].set_pos(camera[f].get_pos());
	helper[f].look_at(0, 0, 0);
	frames[f].set_shader_input("helper", helper[f]);
}


void* panda_thread(void* param){
	int argc = 0;
	char** argv;

	load_prc_file("config.prc");

	pframework = new PandaFramework();
	pframework->open_framework(argc, argv);
	pwindow = pframework->open_window();

	pframework->enable_default_keys();
	pwindow->setup_trackball();
	pwindow->enable_keyboard();

	render = pwindow->get_render();
	scene = render.attach_new_node("Scene Node");
//	scene.set_scale(0.01);
//	scene.set_p(-90);

	width = 640;
	height = 480;

	texture_rgb[0] = new Texture("stream-rgb");
	texture_rgb[1] = new Texture("stream-rgb2");

	PNMImage image = PNMImage(width, height, 1, 65535);
	texture_depth[0] = new Texture("stream-depth");
	texture_depth[0]->load(image);
	texture_depth[1] = new Texture("stream-depth2");
	texture_depth[1]->load(image);

//	NodePath origin = pwindow->load_model(render, "models/smiley");
//	origin.set_scale(0.05);

	camera[0].reparent_to(render);
	camera[1].reparent_to(render);
//	NodePath cam0 = pwindow->load_model(camera[0], "models/camera");
//	NodePath cam1 = pwindow->load_model(camera[1], "models/camera");
//	cam0.set_scale(0.05);
//	cam1.set_scale(0.05);

	pwindow->load_model(helper[0], "models/camera");
	pwindow->load_model(helper[1], "models/camera");

//	camera[0].hide();
//	camera[1].hide();
	helper[0].hide();
	helper[1].hide();

	CardMaker cm = CardMaker("card");
	cm.set_frame_fullscreen_quad();
	NodePath floor = render.attach_new_node(cm.generate());
	floor.set_scale(0.16);
	floor.set_pos(-0.075, 0.11, 0);
//	floor.set_y(0.8);
//	floor.set_p(90);
//	floor.hide();

	create_frame(0);
	create_frame(1);

	frames[0].reparent_to(camera[0]);
	frames[1].reparent_to(camera[1]);


	pframework->main_loop();

	return param;
}



