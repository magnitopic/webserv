def primos_hasta(n):
    prime = [2]
    for i in range(3, n + 1, 2):
        for j in range(3, int(i ** .5) + 1, 2):
            if i % j == 0:
                break
        else:
            prime.append(i)
    return prime


n = 5000
print("<h1>Prime numbers up to", n, "</h1>")
print(*primos_hasta(n))
