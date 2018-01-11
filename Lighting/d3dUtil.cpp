#include "d3dUtil.h"
ID3D11ShaderResourceView* d3dHelper::CreateTexture2DArraySRV( ID3D11Device*device,ID3D11DeviceContext*context, std::vector<std::wstring>&filenames, DXGI_FORMAT format/* =DXGI_FORMAT_FROM_FILE */, UINT filter/* =D3DX11_FILTER_NONE */, UINT mipFilter/* =D3DX11_FILTER_LINEAR */ )
{
	//�����Ĵ��ļ���������Ԫ�أ�������ֱ�ӱ�GPUʹ��(0�󶨱�־)//
	//�������������ļ�����ͼƬ����//
	//ʹ�÷�ʽ��D3D11_USAGE_STAGING����GPU������CPU//
	UINT size=filenames.size();
	std::vector<ID3D11Texture2D*>srcTex(size);
	for (UINT i=0;i<size;++i)
	{
		D3DX11_IMAGE_LOAD_INFO loadInfo;
		loadInfo.Width=D3DX11_FROM_FILE;
		loadInfo.Height=D3DX11_FROM_FILE;
		loadInfo.Depth=D3DX11_FROM_FILE;
		loadInfo.FirstMipLevel=0;
		loadInfo.MipLevels=D3DX11_FROM_FILE;
		loadInfo.Usage=D3D11_USAGE_STAGING;
		loadInfo.BindFlags=0;
		loadInfo.CpuAccessFlags=D3D11_CPU_ACCESS_WRITE|D3D11_CPU_ACCESS_READ;
		loadInfo.MiscFlags=0;
		loadInfo.Format=format;
		loadInfo.MipFilter=mipFilter;
		loadInfo.pSrcInfo=0;
		HR(D3DX11CreateTextureFromFile(device,filenames[i].c_str(),&loadInfo,0,(ID3D11Resource**)&srcTex[i],0));
	}

	//�����������飬�����е�ÿ��Ԫ�ض���һ���ĸ�ʽ/���//

	D3D11_TEXTURE2D_DESC texElementDesc;
	srcTex[0]->GetDesc(&texElementDesc);

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc=texElementDesc;
	
	ID3D11Texture2D*texArray=0;
	HR(device->CreateTexture2D(&texArrayDesc,0,&texArry));
	//��ÿ������Ԫ�ؿ���������//
	for (UINT texElement=0;texElement<size;++texElement)
	{
		for (UINT mipLevel=0;texElement<size;++texElement)
		{
			D3D11_MAPPED_SUBRESOURCE mappedTex2D;
			HR(context->Map(srcTex[texElement],mipLevel,D3D11_MAP_READ,0,&mappedTex2D));////��srcTex[texElement]����ӳ�䵽mappedTex2D��������GPU��srcTex[texElement]����Ȩת����CPU��mappedTex2D�Ĳ����ϣ�mipLevelָ��ת�������ĸ�mip��cpu��mappedTex2D�Ķ�д����ֱ��ӳ�䵽srcTex[texElement]�������ϣ��˴����õ��Ƕ�ȡ����Ȩ�ޡ�������Ҫunmap����ʹGPU�ػ��д�Ĳ���Ȩ�ޡ�
			//UpdateSubresource��ؿ��Կ���http://blog.csdn.net/a359877454/article/details/79024473 //
			context->UpdateSubresource(texArray,D3D11CalcSubresource(mipLevel,texElement,texElementDesc.MipLevels)//D3D��Bufferʵ���ǰ�һά�洢�ģ�������һά����ά������3ά����
				,0,mappedTex2D.pData,mappedTex2D.RowPitch,mappedTex2D.DepthPitch);
			context->Unmap(srcTex[texElement],mipLevel);//����ȡ�������������������//


		}
	}
	//Ϊ�������鴴����ɫ����Դ��ͼ//
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format=texArrayDesc.Format;
	viewDesc.ViewDimension=D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip=0;
	viewDesc.Texture2DArray.MipLevels=texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice=0;
	viewDesc.Texture2DArray.ArraySize=size;
	ID3D11ShaderResourceView*texArraySRV=0;
	HR(device->CreateShaderResourceView(texArray,&viewDesc,&texArraySRV));
	//
	//����--���ǽ���Ҫ������ͼ,��ͼ�����󣬴洢��Դ���ڴ�����ͷ��ˡ�//
	//
	ReleaseCOM(texArray);
	for (UINT i=0;i<size;++i)
	{
		ReleaseCOM(srcTex[i]);
	}

	return texArraySRV;

}
ID3D11ShaderResourceView*d3dHelper::CreateRandomTexture1DSRV(ID3D11Device*device)
{

	//�����������//
	XMFLOAT4 randomValues[1024];
	for (int i=0;i<1024;++i)
	{
		randomValues[i].x=MathHelper::RandF(-1.0f,1.0f);
		randomValues[i].y=MathHelper::RandF(-1.0f,1.0f);
		randomValues[i].z=MathHelper::RandF(-1.0f,1.0f);
		randomValues[i].w=MathHelper::RandF(-1.0f,1.0f);
	}

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem=randomValues;
	initData.SysMemPitch=1024*sizeof(XMFLOAT4);
	initData.SysMemSlicePitch=0;

	//��������//
	D3D11_TEXTURE1D_DESC texDesc;
	texDesc.Width=1024;
	texDesc.MipLevels=1;
	texDesc.Format=DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Usage=D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags=D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags=0;
	texDesc.MiscFlags=0;
	texDesc.ArraySize=1;
	ID3D11Texture1D* randomTex=0;
	HR(device->CreateTexture1D(&texDesc,&initData,&randomTex));

	//������ͼ//
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format=texDesc.Format;
	viewDesc.ViewDimension=D3D11_SRV_DIMENSION_TEXTURE1D;
	viewDesc.Texture1D.MipLevels=texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip=0;

	ID3D11ShaderResourceView*randomTexSRV=0;
	HR(device->CreateShaderResourceView(randomTex,&viewDesc,&randomTexSRV));

	ReleaseCOM(randomTex);


	return randomTexSRV;

}
//��ȡƽ��ͷ�����//
void ExtractFrustumPlanes(XMFLOAT4 planes[6],CXMMATRIX M)
{

	//
	// Left
	//
	planes[0].x = M(0,3) + M(0,0);
	planes[0].y = M(1,3) + M(1,0);
	planes[0].z = M(2,3) + M(2,0);
	planes[0].w = M(3,3) + M(3,0);

	//
	// Right
	//
	planes[1].x = M(0,3) - M(0,0);
	planes[1].y = M(1,3) - M(1,0);
	planes[1].z = M(2,3) - M(2,0);
	planes[1].w = M(3,3) - M(3,0);

	//
	// Bottom
	//
	planes[2].x = M(0,3) + M(0,1);
	planes[2].y = M(1,3) + M(1,1);
	planes[2].z = M(2,3) + M(2,1);
	planes[2].w = M(3,3) + M(3,1);

	//
	// Top
	//
	planes[3].x = M(0,3) - M(0,1);
	planes[3].y = M(1,3) - M(1,1);
	planes[3].z = M(2,3) - M(2,1);
	planes[3].w = M(3,3) - M(3,1);

	//
	// Near
	//
	planes[4].x = M(0,2);
	planes[4].y = M(1,2);
	planes[4].z = M(2,2);
	planes[4].w = M(3,2);

	//
	// Far
	//
	planes[5].x = M(0,3) - M(0,2);
	planes[5].y = M(1,3) - M(1,2);
	planes[5].z = M(2,3) - M(2,2);
	planes[5].w = M(3,3) - M(3,2);

	//��һ��ƽ�淽��//
	for (int i=0;i<6;++i)
	{
		XMVECTOR v=XMPlaneNormalize(XMLoadFloat4(&planes[i]));
		XMStoreFloat4(&planes[i],v);
	}
}