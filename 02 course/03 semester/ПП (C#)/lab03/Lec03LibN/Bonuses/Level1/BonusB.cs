namespace Lec03LibN.Bonuses.Level1
{
    public class BonusB : IBonus
    {
        public float cH { get; set; }
        public float x { get; set; }

        public BonusB(float cH, float x)
        {
            this.cH = cH;
            this.x = x;
        }

        public float calc(float wH)
        {
            return wH * cH * x;
        }
    }
}
