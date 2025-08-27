using Lec03LibN.Bonuses.Level1;

namespace Lec03LibN.FactoryLevels
{
    public class FactoryLevel1 : IFactory
    {
        public IBonus getA(float cH) => new BonusA(cH);

        public IBonus getB(float cH, float x) => new BonusB(cH, x);

        public IBonus getC(float cH, float x, float y) => new BonusC(cH, x, y);
    }
}
