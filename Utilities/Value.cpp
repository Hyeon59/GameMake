#include "stdafx.h"
#include "Value.h"

D3DXVECTOR2 Value::Zero2	= D3DXVECTOR2(0.0f, 0.0f);
D3DXVECTOR3 Value::Zero		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 Value::One		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);

D3DXVECTOR3 Value::Up		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
D3DXVECTOR3 Value::Down		= D3DXVECTOR3(0.0f, -1.0f, 0.0f);
D3DXVECTOR3 Value::Left		= D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
D3DXVECTOR3 Value::Right	= D3DXVECTOR3(1.0f, 0.0f, 0.0f);
D3DXVECTOR3 Value::Front	= D3DXVECTOR3(0.0f, 0.0f, 1.0f);

D3DXCOLOR Value::Red		= D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
D3DXCOLOR Value::Green		= D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
D3DXCOLOR Value::Blue		= D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
D3DXCOLOR Value::Yellow     = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
D3DXCOLOR Value::Black		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
D3DXCOLOR Value::White		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

D3DXMATRIX Value::Identity = {  1, 0, 0, 0,
								0, 1, 0, 0,
								0, 0, 1, 0,
								0, 0, 0, 1 };
