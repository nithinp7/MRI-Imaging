#version 430

#define WIDTH 256
#define DEPTH 99
#define HEIGHT 256

const ivec3 cornerTable [] = {
		ivec3(0, 0, 0),
		ivec3(1, 0, 0),
		ivec3(1, 1, 0),
		ivec3(0, 1, 0),
		ivec3(0, 0, 1),
		ivec3(1, 0, 1),
		ivec3(1, 1, 1),
		ivec3(0, 1, 1)
};
const vec3 edgeTable[][2] = {

		{ vec3(0.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0) },
		{ vec3(1.0, 0.0, 0.0), vec3(1.0, 1.0, 0.0) },
		{ vec3(0.0, 1.0, 0.0), vec3(1.0, 1.0, 0.0) },
		{ vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0) },
		{ vec3(0.0, 0.0, 1.0), vec3(1.0, 0.0, 1.0) },
		{ vec3(1.0, 0.0, 1.0), vec3(1.0, 1.0, 1.0) },
		{ vec3(0.0, 1.0, 1.0), vec3(1.0, 1.0, 1.0) },
		{ vec3(0.0, 0.0, 1.0), vec3(0.0, 1.0, 1.0) },
		{ vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0) },
		{ vec3(1.0, 0.0, 0.0), vec3(1.0, 0.0, 1.0) },
		{ vec3(1.0, 1.0, 0.0), vec3(1.0, 1.0, 1.0) },
		{ vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 1.0) }
};
const int edgeToCornerTable[][2] = {
	{ 0, 1 },
	{ 1, 2 },
	{ 3, 2 },
	{ 0, 3 },
	{ 4, 5 },
	{ 5, 6 },
	{ 7, 6 },
	{ 4, 7 },
	{ 0, 4 },
	{ 1, 5 },
	{ 2, 6 },
	{ 3, 7 }
};

layout(binding = 0) uniform usamplerBuffer voxels;
layout(binding = 1) uniform isamplerBuffer triTableTex;

uniform bool geomBypass;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//uniform mat4 smd;

uniform float par_t;

in ivec3 gridPos[];

out vec3 FragPos;
out vec3 Normal;
out vec4 Color;

int cube[8];

int get_voxel(int x, int y, int z)
{
	x = (x + 128) % 256;
	y = (256 - y);

	//return voxels[z * WIDTH * HEIGHT + y * WIDTH + x];
	return int(texelFetch(voxels, z * WIDTH * HEIGHT + y * WIDTH + x).r);
}

float get_voxel_safe(int x, int y, int z) {

	if (x < 0 || y < 0 || z < 0 ||
		x >= WIDTH || y >= HEIGHT || z >= DEPTH)
		return 0.0;
	
	//if (z < par_t) return 0.0;

	return float(get_voxel(x, y, z));
}

void get_cube(ivec3 upos)
{
	for (int i = 0; i < 8; i++)
	{
		ivec3 v = upos + cornerTable[i];
		cube[i] = get_voxel(v.x, v.y, v.z);
	}
}

int triTable(int i, int j)
{
	return int(texelFetch(triTableTex, i*16 + j).r);
}

vec3 get_gradient(int x, int y, int z) {
	return normalize(vec3(
		get_voxel_safe(x + 1, y, z) - get_voxel_safe(x - 1, y, z),
		get_voxel_safe(x, y + 1, z) - get_voxel_safe(x, y - 1, z),
		get_voxel_safe(x, y, z + 1) - get_voxel_safe(x, y, z - 1)
	));
}

layout(points) in;
layout(triangle_strip, max_vertices = 15) out;
void main()
{
	/**/
	Normal = vec3(0.0, 0.0, 1.0);
	Color = vec4(1.0, 0.0, 0.0, 1.0);
	mat4 mvp = projection * view * model;
	vec4 gPos = mvp * vec4(float(gridPos[0].x), float(gridPos[0].y), float(gridPos[0].z), 0.0);
	
	vec3 pos = vec3(float(gridPos[0].x), float(gridPos[0].y), float(gridPos[0].z));
	
	float threshold = 40.0;
	float limit = 90.0;
	
	//if(pos.z < par_t) return;

	vec3 offs = vec3(0.0, 0.0, 0.0);

	get_cube(gridPos[0]);
	
	int config = 0;
	for (int i = 0; i < 8; i++) 
		if (cube[i] > threshold)
			config |= 1 << i;

	if (config == 0 || config == 255)
		return;
	
	//vec3 pos = vec3(gPos.x, gPos.y, 0.5 * gPos.z);
	int edge = 0;
	bool cont = false;
	for (int i = 0; i < 5; i++)
	{
		for (int p = 0; p < 3; p++)
		{
			int index = triTable(config, edge);
						
			if (index == -1)
			{
				cont = true;
				break;
			}

			int i0 = edgeToCornerTable[index][0];
			int i1 = edgeToCornerTable[index][1];

			ivec3 c0 = cornerTable[i0];
			ivec3 c1 = cornerTable[i1];

			float d0 = cube[i0] - threshold;
			float d1 = cube[i1] - threshold;

			vec3 v0 = edgeTable[index][0];
			vec3 v1 = edgeTable[index][1];
						
			//vec3 vertPos = offs + pos + 0.5f  * edgeTable[index][0] + 0.5f * edgeTable[index][1];
			//vec3 vertPos = offs + pos + -d0 * (v1 - v0) / (d1 - d0) + v0;
			vec3 vertPos = offs + pos + -d0 * (v1 - v0) / (d1 - d0) + v0;

			vec3 g0 = get_gradient(gridPos[0].x + c0.x, gridPos[0].y + c0.y, gridPos[0].z + c0.z);
			vec3 g1 = get_gradient(gridPos[0].x + c1.x, gridPos[0].y + c1.y, gridPos[0].z + c1.z);

			vec3 g = -d0 * (g1 - g0) / (d1 - d0) + g0;
				
			//FragPos = vec3(Model * vec4(vertPos.x / float(WIDTH), vertPos.y / float(HEIGHT), vertPos.z / float(DEPTH), 1.0));
			//Normal = normalize(g);
			
			FragPos = (model * vec4(0.1 * vertPos, 1.0)).xyz;//vec3(model * vec4(vertPos, 1.0));
			Normal = mat3(transpose(inverse(model))) * normalize(g);  
			Color = vec4(0.8, 0.4, 0.6, 1.0);

			//FragPos = vec3(float(gridPos[0].x + i), float(gridPos[0].y + i*i), float(gridPos[0].z));
			//Normal = vec3(0.0, 0.0, 1.0);
			//Color = vec4(1.0, 0.0, 0.0, 1.0);
						
			//mat4 pr = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
			gl_Position = mvp * vec4(0.1 * vertPos, 1.0);//vec4(vertPos, 1.0);
						
			//gl_Position = vec4(FragPos, 1.0);
			//gl_Position = vec4(vertPos, 1.0);
			//vertices.push_back(make_vertex(vertPos, g));

			EmitVertex();
						
			edge++;
		}

		if(cont) break;

		EndPrimitive();

		//unsigned int vs = vertices.size();
		//set_normals(vertices[vs - 3], vertices[vs - 2], vertices[vs - 1]);
	}
	/**/
}