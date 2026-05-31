function Sum(a, b) {
    return a + b;
}

function Mul(a, b) {
    return a * b;
}

function Div(a, b) {
    if (b === 0) {
        throw new Error("Division by zero is not allowed.");
    }
    return a / b;
}

function Sub(a, b) {
    return a - b;
}