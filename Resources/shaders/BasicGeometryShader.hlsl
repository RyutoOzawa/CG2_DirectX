#include"Obj.hlsli"



//点の入力から、四角形を出力
[maxvertexcount(3)]
void main(
	triangle VSOutput input[3]:SV_POSITION,
	inout TriangleStream<GSOutput> output
)
{
	
	//4頂点分回す
	for (uint i = 0; i < 3; i++) {
		GSOutput element;
		//ワールド座標ベースでずらす
		element.svpos =input[i].svpos;
		element.normal =input[i].normal;
		element.uv =input[i].uv;
		output.Append(element);
	}
}