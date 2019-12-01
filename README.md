# dx11_particle

# Overview
ジオメトリインスタンシングとジオメトリーシェーダを使ってパーティクルシステム
# Requirement
Shader Model 5.0
# Installation
ビールボード機能を有効するために、constantbufferでカメラワルド座標を送らなければならないです。

その初期化の仕方はgame.cppにこんな風に書けばなんとなると思います。
```c++
particleTest = AddGameObject<CParticle>();
XMFLOAT3* movement = new XMFLOAT3[particleTest->GetInstanceCount()];

for (int i = 0; i < particleTest->GetInstanceCount(); i++) {
	movement[i].x = cos((i - 1))*0.05f; //何番目のインスタンスがこういうｘ動きする
	movement[i].y = .05f;               //何番目のインスタンスがこういうｙ動きする
	movement[i].z = sin((i - 1))*0.05f; //何番目のインスタンスがこういうｚ動きする
}
particleTest->SetMaxFrame(60);
particleTest->SetMovement(movement);
```

位置更新するために、constantbufferでパーティクルの位置データも送らなければならないですが、cppにもうすでに書いています。自分のプログラムに空いてるconstantbufferのスロットにあわせばいいです。

# License
GPL [https://www.gnu.org/licenses/gpl-3.0.en.html]
