def rechnen(*args):
    result = 0
    for expr in args:
        if '*' in expr or '/' in expr:
            return "[RUNTIME ERROR]: Da Gazo aus der Volkschule gedropped ist kann er nur Addition und Subtraktion durchführen"
        eval_result = eval(expr)
        if -100 <= eval_result <= 100:
            result += eval_result
        else:
            return "[RUNTIME ERROR]: Da Gazo aus der Volkschule gedropped ist kann er nur Rechnungen mit Ergebnissen zwischen -100 und 100 durchführen"
    return result

def say(*args):
    output = ' '.join(map(str, args))
    print(output)