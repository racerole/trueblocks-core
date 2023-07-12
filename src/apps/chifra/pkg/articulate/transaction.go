package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (cache *AbiCache) ArticulateTx(chain string, tx *types.SimpleTransaction) (err error) {
	address := tx.To
	if !cache.loadedMap[address] && !cache.skipMap[address] {
		if err := abi.LoadAbi(chain, address, cache.abiMap); err != nil {
			cache.skipMap[address] = true
			return err
		} else {
			cache.loadedMap[address] = true
		}
	}

	if tx.Receipt != nil {
		for index := range tx.Receipt.Logs {
			if err = cache.ArticulateLog(chain, &tx.Receipt.Logs[index]); err != nil {
				return err
			}
		}
	}

	for index := range tx.Traces {
		if err = cache.ArticulateTrace(chain, &tx.Traces[index]); err != nil {
			return err
		}
	}

	var found *types.SimpleFunction
	var selector string
	if len(tx.Input) >= 10 {
		selector = tx.Input[:10]
		inputData := tx.Input[10:]
		found = cache.abiMap[selector]
		if found != nil {
			tx.ArticulatedTx = found.Clone()
			var outputData string
			if len(tx.Traces) > 0 && tx.Traces[0].Result != nil && len(tx.Traces[0].Result.Output) > 2 {
				outputData = tx.Traces[0].Result.Output[2:]
			}
			if err = ArticulateFunction(tx.ArticulatedTx, inputData, outputData); err != nil {
				return err
			}
		}
	}

	if found == nil && len(tx.Input) > 0 {
		if message, ok := ArticulateString(tx.Input); ok {
			tx.Message = message
			// } else if len(selector) > 0 {
			// 	// don't report this error
			// 	errorChan <- fmt.Errorf("method/event not found: %s", selector)
		}
	}

	return nil
}
