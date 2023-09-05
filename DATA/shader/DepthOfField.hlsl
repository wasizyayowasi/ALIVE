////ピクセルシェーダの入力
//struct PS_INPUT{
//	float2 texcoord : TEXCOORD0;
//};
//
////ピクセルシェーダーの出力
//struct PS_OUTPUT {
//	float4 color : COLOR0;
//};
//
//float2 DOF_Focus : register(c0);	//被写界深度のフォーカス情報 x:ファーカス開始位置　y:フォーカス終了位置
//
//sampler normalTexture				:register(s0);		//くっちりテクスチャ
//sampler weakVertigoTexture			:register(s1);		//弱いぼかしのテクスチャ
//sampler strongVertigoTexture		:register(s2);		//強いぼかしのテクスチャ
//sampler depthMapTexture				:register(s3);		//深度マップテクスチャ


//PS_OUTPUT main(PS_INPUT PSInput)
float4 main() : SV_TARGET
{
	//PS_OUTPUT PSOutput;
	//float4 weakVertigoTextureColor, strongVertigoColor;
	//float blendRate;
	//float fade;
	//float depth;

	////深度地を取得する
	//depth = tex2D(depthMapTexture, PSInput.texcoord);

	////フォーカス情報からぼやけ率を産出する
	//if (depth < DOF_Focus.x) {
	//	fade = 1.0 - depth / DOF_Focus.x;
	//}
	//else if(depth < DOF_Focus.y) {
	//	fade = 0.0f;
	//}
	//else {
	//	fade = (depth - DOF_Focus.y) / (1.0f - DOF_Focus.y);
	//}

	//if (fade < 0.5f) {
	//	weakVertigoTextureColor = tex2D(normalTexture, PSInput.texcoord);
	//	strongVertigoColor = tex2D(weakVertigoTexture, PSInput.texcoord);
	//	blendRate = (fade - 0.5f) / 0.5f;
	//}
	//else {
	//	weakVertigoTextureColor = tex2D(weakVertigoTexture, PSInput.texcoord);
	//	strongVertigoColor = tex2D(strongVertigoTexture, PSInput.texcoord);
	//	blendRate = (fade - 0.5f) / 0.5f;
	//}

	//PSOutput.color = lerp(weakVertigoTextureColor, strongVertigoColor, blendRate);

	//return PSOutput;
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}