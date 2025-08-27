using Lec03LibN.Bonuses.Level2;

namespace Lec03LibN.FactoryLevels
{
    public class FactoryLevel2 : IFactory
    {
        public float a { get; set; }

        public FactoryLevel2(float a)
        {
            this.a = a;
        }

        public IBonus getA(float cH) => new BonusA(cH, a);

        public IBonus getB(float cH, float x) => new BonusB(cH, x, a);

        public IBonus getC(float cH, float x, float y) => new BonusC(cH, x, y, a);
    }
}
