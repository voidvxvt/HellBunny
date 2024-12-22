package main

import (
    "crypto/aes"
    "crypto/cipher"
    "crypto/rand"
    "fmt"
    "io"
    "os"
)

func XCryptBytesAES256(XBytes []byte, AESKey []byte, AESIv []byte) []byte {
    var ReverseXBytes = make([]byte, len(XBytes))

    block, err := aes.NewCipher(AESKey)
    if err != nil {
        fmt.Println("Encryption/Decryption Error: " + err.Error())
        return nil
    }

    stream := cipher.NewCTR(block, AESIv)
    stream.XORKeyStream(ReverseXBytes, XBytes)

    return ReverseXBytes
}

func formatBytesAsHex(bytes []byte) string {
    formatted := "{ "
    for i, b := range bytes {
        if i > 0 {
            formatted += ", "
        }
        formatted += fmt.Sprintf("0x%02x", b)
    }
    formatted += " }"
    return formatted
}

func main() {
    if len(os.Args) < 2 {
        fmt.Printf("Usage: %s <file>\n", os.Args[0])
    }

    filePath := os.Args[1]

    file, err := os.Open(filePath)
    if err != nil {
        fmt.Printf("Failed opening file: %s\n", err)
    }
    defer file.Close()

    data, err := io.ReadAll(file)
    if err != nil {
        fmt.Printf("Failed reading file: %s\n", err)
    }

    // Generate random AES key and IV
    AesKey := make([]byte, 32) // AES-256 key size
    AesIv := make([]byte, aes.BlockSize)

    _, err = rand.Read(AesKey)
    if err != nil {
        fmt.Printf("Failed generating random key: %s\n", err)
    }
    _, err = rand.Read(AesIv)
    if err != nil {
        fmt.Printf("Failed generating random IV: %s\n", err)
    }

    cryptData := XCryptBytesAES256(data, AesKey, AesIv)

    outfilePath := filePath + ".enc"

    err = os.WriteFile(outfilePath , cryptData, 0644)
    if err != nil {
        fmt.Printf("Failed writing file: %s\n", err)
    }

    fmt.Printf("Encrypted data written to %s\n", outfilePath)
    fmt.Printf("AES Key: %s\n", formatBytesAsHex(AesKey))
    fmt.Printf("AES IV: %s\n", formatBytesAsHex(AesIv))
}

// go build -o Aes256CtrXCrypt ./main.go
// Aes256CtrXCrypt ./shc.bin