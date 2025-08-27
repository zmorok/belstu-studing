using Lec03LibN.Bonuses.Level3;

namespace Lec03LibN.FactoryLevels
{
    public class FactoryLevel3 : IFactory
    {
        public float a { get; set; }
        public float b { get; set; }

        public FactoryLevel3(float a, float b)
        {
            this.a = a;
            this.b = b;
        }

        public IBonus getA(float cH) => new BonusA(cH, a, b);

        public IBonus getB(float cH, float x) => new BonusB(cH, x, a, b);

        public IBonus getC(float cH, float x, float y) => new BonusC(cH, x, y, a, b);
    }
}
