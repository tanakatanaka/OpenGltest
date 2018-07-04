#version 400

layout( triangles_adjacency ) in;
layout( triangle_strip, max_vertices = 3 ) out;

out vec3 GNormal;
out vec3 GPosition;
noperspective out vec3 GEdgeDistance;

in vec3 VNormal[];
in vec3 VPosition[];

uniform mat4 ViewportMatrix;  // Viewport matrix

//silhoute用
flat out int GIsEdge;
uniform float EdgeWidth;
uniform float PctExtend;


bool isFrontFacing( vec3 a, vec3 b, vec3 c )
{
    return ((a.x * b.y - b.x * a.y) + (b.x * c.y - c.x * b.y) + (c.x * a.y - a.x * c.y)) > 0;
}

void emitEdgeQuad( vec3 e0, vec3 e1 )
{
    vec2 ext = PctExtend * (e1.xy - e0.xy);
    vec2 v = normalize(e1.xy - e0.xy);
    vec2 n = vec2(-v.y, v.x) * EdgeWidth;

    //GIsEdge = 1;   // This is part of the sil. edge

    gl_Position = vec4( e0.xy - ext, e0.z, 1.0 ); EmitVertex();
    gl_Position = vec4( e0.xy - n - ext, e0.z, 1.0 ); EmitVertex();
    gl_Position = vec4( e1.xy + ext, e1.z, 1.0 ); EmitVertex();
    gl_Position = vec4( e1.xy - n + ext, e1.z, 1.0 ); EmitVertex();

    EndPrimitive();
}

void main()
{
    // Transform each vertex into viewport space
    vec2 p0 = vec2(ViewportMatrix * (gl_in[0].gl_Position / gl_in[0].gl_Position.w));
    vec2 p1 = vec2(ViewportMatrix * (gl_in[1].gl_Position / gl_in[1].gl_Position.w));
    vec2 p2 = vec2(ViewportMatrix * (gl_in[2].gl_Position / gl_in[2].gl_Position.w));

	vec3 p0a = gl_in[0].gl_Position.xyz / gl_in[0].gl_Position.w;
    vec3 p1a = gl_in[1].gl_Position.xyz / gl_in[1].gl_Position.w;
    vec3 p2a = gl_in[2].gl_Position.xyz / gl_in[2].gl_Position.w;
    vec3 p3a = gl_in[3].gl_Position.xyz / gl_in[3].gl_Position.w;
    vec3 p4a = gl_in[4].gl_Position.xyz / gl_in[4].gl_Position.w;
    vec3 p5a = gl_in[5].gl_Position.xyz / gl_in[5].gl_Position.w;

    //if( isFrontFacing(p0a, p2a, p4a) ) 
    {
        if( ! isFrontFacing(p0a,p1a,p2a) ) emitEdgeQuad(p0a,p2a);
        //if( ! isFrontFacing(p2a,p3a,p4a) ) emitEdgeQuad(p2a,p4a);
        //if( ! isFrontFacing(p4a,p5a,p0a) ) emitEdgeQuad(p4a,p0a);
    }
	
    float a = length(p1 - p2);
    float b = length(p2 - p0);
    float c = length(p1 - p0);
    float alpha = acos( (b*b + c*c - a*a) / (2.0*b*c) );
    float beta = acos( (a*a + c*c - b*b) / (2.0*a*c) );
    float ha = abs( c * sin( beta ) );
    float hb = abs( c * sin( alpha ) );
    float hc = abs( b * sin( alpha ) );

    GEdgeDistance = vec3( ha, 0, 0 );
    GNormal = VNormal[0];
    GPosition = VPosition[0];
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    GEdgeDistance = vec3( 0, hb, 0 );
    GNormal = VNormal[1];
    GPosition = VPosition[1];
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    GEdgeDistance = vec3( 0, 0, hc );
    GNormal = VNormal[2];
    GPosition = VPosition[2];
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}
