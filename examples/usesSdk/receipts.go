package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoReceipts tests the When sdk function
func DoReceipts() {
	logger.Info("DoReceipts")

	opts := sdk.ReceiptsOptions{
		TransactionIds: []string{"10001002.0"},
	}

	buf := bytes.Buffer{}
	if err := opts.ReceiptsBytes(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/receipts.json", buf.String())
	fmt.Println(buf.String())
}
// func (opts *ReceiptsOptions) Receipts() ([]types.Receipt, *types.MetaData, error) {
