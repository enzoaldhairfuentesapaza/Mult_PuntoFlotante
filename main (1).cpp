#include <iostream>
#include <bitset>
using namespace std;
bitset<23> Redondeo(bitset<49>j)
{
  bitset<23>redondeo(0);
  for(int i=0; i<23; i++)
  {
    redondeo[i]=j[i];
  }
  return redondeo;
}
bitset<49> SumaBinaria(bitset<49>x, bitset<49>y)
{
  bitset<49> suma;
  bool acarreo = 0;
  for (int i = 0; i < 49; i++) 
  {
    bool bitA = x[i];
    bool bitB = y[i];
    bool bitS = bitA ^ bitB ^ acarreo;
    acarreo = (bitA && bitB) || (bitA & acarreo) || (bitB && acarreo);
    suma[i] = bitS;
  }
  return suma;
}

bitset<8> SumaBiased(bitset<8>x, bitset<8>y)
{
  bitset<8> Bias("10000001");
  bitset<8> Suma2Bias;
  bitset<8> SumaFinal;
  bool acarreo = 0;
  for (int i = 0; i < 8; i++) 
  {
    bool bitA = x[i];
    bool bitB = y[i];
    bool bitS = bitA ^ bitB ^ acarreo;
    acarreo = (bitA && bitB) || (bitA & acarreo) || (bitB && acarreo);
    Suma2Bias[i] = bitS;
  }
  acarreo = 0;
  for (int i = 0; i < 8; i++) 
  {
    bool bitA = Suma2Bias[i];
    bool bitB = Bias[i];
    bool bitS = bitA ^ bitB ^ acarreo;
    acarreo = (bitA && bitB) || (bitA && acarreo) || (bitB && acarreo);
    SumaFinal[i] = bitS;
  }
  return SumaFinal;
}
bitset<8> Normalizar(bitset<49> &retorno)
{
  int n=48;
  while(!(retorno[n]))
  {
    n--;
  }
  if(n-23 <= 0)
  {
    retorno<<=(n-23);
  }
  else
  {
      retorno>>=(n-23);
  }
  int NumB = (n-46)+127;
  bitset<8> SumaExp(NumB);
  return SumaExp;
}
bitset<49> MultSignificando(bitset<49> a, bitset<49> b)
{
  a[23]=1;
  b[23]=1;
  bitset<49>SumaAcumulada(0);
  for(int i=0; i<23+1; i++)
  {
    if(b[0])
    {
      SumaAcumulada=SumaBinaria(a,SumaAcumulada);
    }
    a<<=1;
    b>>=1;
  }
  return SumaAcumulada;
}

union Float32Bits {
    float floatValue;
    struct {
        uint32_t significando : 23;
        uint32_t exponente : 8;
        uint32_t signo : 1;
    } parts;
};

int main() {
    Float32Bits value1, value2;
    cout << "Ingresa el primer valor en formato float: ";
    cin >> value1.floatValue;
    cout << "Ingresa el segundo valor en formato float: ";
    cin >> value2.floatValue;

    bitset<1>signo1(value1.parts.signo);
    bitset<8>exp1(value1.parts.exponente);
    bitset<23>sig1(value1.parts.significando);
  
    bitset<1>signo2(value2.parts.signo);
    bitset<8>exp2(value2.parts.exponente);
    bitset<23>sig2(value2.parts.significando);

    cout << "Primer valor:" << endl;
    cout << "Signo: " <<  signo1 << endl;
    cout << "Exponente: " << exp1  << endl;
    cout << "Significando: " << sig1 << endl;

    cout << "Segundo valor:" << endl;
    cout << "Signo: " << signo2 << endl;
    cout << "Exponente: " << exp2 << endl;
    cout << "Significando: " << sig2 << endl;
  
    if((exp1.none() && sig1.none()) || (exp2.none() && sig2.none()))
    {
      cout << "El resultado es: \"0\"" << endl;
      cout << "En bits es: "<< bitset<1>(0) << " " << bitset<8>(0) << " " << bitset<23>(0);
    }

    bitset<8>SumaExp= SumaBiased(exp1, exp2);
    if((exp1[7]==0 && exp2[7]==0) && SumaExp[7]==1)
    {
      cout << "underflow de exponente NAN";
      return 0;
    }
    else if ((exp1[7]==1 && exp2[7]==1) && SumaExp[7]==0)
    {
      cout << "overflow de exponente NAN";
      return 0;
    }
    bitset<49>multSig = MultSignificando(bitset<49>(value1.parts.significando),bitset<49>(value2.parts.significando));

    bitset<8>AdiExp = Normalizar(multSig);

    bitset<23>SigFinal = Redondeo(multSig);

    bitset<8>ExpFinal = SumaBiased(AdiExp, SumaExp);

    if((AdiExp[7]==0 && SumaExp[7]==0) && ExpFinal[7]==1)
    {
      cout << "2 underflow de exponente NAN";
      return 0;
    }
    else if ((AdiExp[7]==1 && SumaExp[7]==1) && ExpFinal[7]==0)
    {
      cout << "2 overflow de exponente NAN";
      return 0;
    }
    bitset<1>SignoFinal = signo1 ^= signo2;
  cout << "EL RESULTADO DEL PRODUCTO ES: " << SignoFinal << " " << SumaExp << " " << SigFinal << endl;
  return 0;
}
