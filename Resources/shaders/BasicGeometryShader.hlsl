#include"Obj.hlsli"

[maxvertexcount(1)]
void main(
	point VSOutput input[1],
	inout PointStream<GSOutput> output
)
{
	
    GSOutput element;
	//’¸“_î•ñ
    element.normal = input[0].normal;
    element.uv = input[0].uv;
    element.svpos = input[0].svpos;
    output.Append(element);
}