namespace Lempel_Ziv_method;

// Результат поиска совпадения в словаре
struct MatchResult
{
    public int Position { get; set; }
    public int Length { get; set; }
}

// Закодированный токен (триада) алгоритма LZ77
struct EncodedToken
{
    public string Position { get; set; }
    public string Length { get; set; }
    public string NextChar { get; set; }
}