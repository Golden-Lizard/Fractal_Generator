#version 330 core
in vec2 pos;
in vec2 UV;

uniform float scale;
uniform vec2 position;
uniform float zoom;

uniform vec2 center;
uniform vec2 size;
uniform vec3 rands;

out vec4 color;

float _sqr(float nr)
{
   return pow(nr,2);
}

float sum(vec2 img)
{
   return float(_sqr(img.x)+_sqr(img.y));
}

float complex_sqr(vec2 img)
{
   return float(_sqr(img.x) - _sqr(img.y));
}

vec3 gaussian_blur(vec3 color)
{
	float gaussian_kernel[] = {0.0f,0.04779,0.904419,0.04779,0.0f};

	float normalizing_factor =0.0f;

	int mSize = 5;
	int kSize = int((mSize-1)/2);

	for(int i=0; i<mSize;++i)
	 normalizing_factor+=gaussian_kernel[i];

	for(int i=-kSize; i<=kSize;++i)
	 for(int j=-kSize; j<=kSize;++j)
	   color+=gaussian_kernel[kSize+j]*gaussian_kernel[kSize+i]*color;

	return color;
}

void main()
{
    vec2 uv = gl_FragCoord.xy / size;
	uv=((uv+position)*zoom);

	uv.y*=scale;
	uv.y+=0.0f;
	uv.x-=0.5f;

	vec2 complex = vec2(0.0f);
	vec3 color = vec3(0.0f);

	float iter=0.0f;
	for(; int(iter)<0x100; iter+=1.0f)
	{

		if((sum(complex) >= 4.0)) break; 
		complex = vec2(complex_sqr(complex), 2.0*complex.y*complex.x) + uv; 

		if(sum(complex) >= 2.0) 
		{
			color.b=iter/4.0;
			color.g=sin((iter/25.5f));
		}
	}

	if(int(iter) < 0x100)
	{
	    float log_zn = float(log( _sqr(complex.x) + _sqr(complex.y)));
		float nu = float(log( log_zn / log(2) ) / 2.0);
		iter=iter+1-nu;
	};

	vec3 color2 = vec3(0.0f);
	color2.b= float(iter+1.0f)/4.0;
	color2.g= sin((float(iter+1.0f)/25.5));

	float r =  0.0f, g = 0.0f, b  = 0.0f;
	
	color = mix(color * rands.xyz, color2, iter - float(int(iter))); 

   /*
	for(int v=0; v < 4; ++v)
	  for(int u=0; u < 4; ++u)
	  {
		     float fx = float(gl_FragCoord.x)+float(u) / 4.0f;
			 float fy = float(gl_FragCoord.y)+float(v) / 4.0f;
		       
			 r+=color.r;
			 g+=color.g;
			 b+=color.b;
	 }

	 r/=16.0f;
	 g/=16.0f;
	 b/=16.0f;
	 */

	//color = gaussian_blur(color);

	gl_FragColor = vec4(color, 1.0f);

}