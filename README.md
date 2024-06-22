# DirectX11를 활용한 포트폴리오

DirectX11와 ImGui를 활용하여 모델 렌더링을 수행하는 프로젝트입니다.
프로젝트 관련 영상은 아래 링크를 통하여 동영상으로 확인하실 수 있습니다.
1. 모델 렌더링
- https://youtu.be/g-0dwWrTKjM
>  모델을 렌더링 할 때, 세가지 조명에 대한 Pixel Shader를 구분하여 코드를 작성하였습니다.
>  주변광의 경우 Image Based Lighting을 활용하여 표현하였고, 직접광의 경우  Microfacet BRDF를 활용하여 Point Light와 Spot Light를 구분하여 표현하였습니다. 
2. 거울 렌더링
- https://youtu.be/VI1tqo4TE00
> 일반적으로 거울은 그려져야 하는 영역에 스텐실 처리를 하고 반사된 위치로 다른 물체의 변환행렬을 업데이트한 뒤 스텐실 처리 된 영역에 대해서 다시 그리는 방식으로 표현합니다. 하지만 제 프로젝트에서는 거울을 하나의 카메라처럼 처리하여, Render Target View를 업데이트하고 해당 RTV와 연관된 SRV를 해당 거울의 텍스쳐로 사용하였습니다.
3. Spot Light
- https://youtu.be/WcNoGSo2Nfk
4. Point Light
- https://youtu.be/sDJgnIE5PN4
> Point Light의 경우, 6방향 그림자맵을 큐브맵으로 활용하여 전방향에 대한 그림자 처리를 수행하였습니다.

해당 프로젝트를 통하여 기본적인 렌더링 엔진 구조에 대한 이해가 더 필요하다고 생각되었습니다.
현재 언리얼 엔진 5를 공부하며 기본적인 게임 엔진 구조를 파악하고 클라이언트 프로그래밍에 대한 이해를 넓히고 있으며,  DirectX12를 통하여 엔진 개발을 추가적으로 수행할 예정입니다.
