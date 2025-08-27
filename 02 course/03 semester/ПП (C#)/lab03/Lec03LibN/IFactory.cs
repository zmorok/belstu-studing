namespace Lec03LibN
{
    public interface IFactory
    {
        IBonus getA(float cH);
        IBonus getB(float cH, float x);
        IBonus getC(float cH, float x, float y);
    }
}
