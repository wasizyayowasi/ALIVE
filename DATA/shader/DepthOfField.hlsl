////�s�N�Z���V�F�[�_�̓���
//struct PS_INPUT{
//	float2 texcoord : TEXCOORD0;
//};
//
////�s�N�Z���V�F�[�_�[�̏o��
//struct PS_OUTPUT {
//	float4 color : COLOR0;
//};
//
//float2 DOF_Focus : register(c0);	//��ʊE�[�x�̃t�H�[�J�X��� x:�t�@�[�J�X�J�n�ʒu�@y:�t�H�[�J�X�I���ʒu
//
//sampler normalTexture				:register(s0);		//��������e�N�X�`��
//sampler weakVertigoTexture			:register(s1);		//�ア�ڂ����̃e�N�X�`��
//sampler strongVertigoTexture		:register(s2);		//�����ڂ����̃e�N�X�`��
//sampler depthMapTexture				:register(s3);		//�[�x�}�b�v�e�N�X�`��


//PS_OUTPUT main(PS_INPUT PSInput)
float4 main() : SV_TARGET
{
	//PS_OUTPUT PSOutput;
	//float4 weakVertigoTextureColor, strongVertigoColor;
	//float blendRate;
	//float fade;
	//float depth;

	////�[�x�n���擾����
	//depth = tex2D(depthMapTexture, PSInput.texcoord);

	////�t�H�[�J�X��񂩂�ڂ₯�����Y�o����
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