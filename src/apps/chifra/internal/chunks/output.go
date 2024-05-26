// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package chunksPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunChunks handles the chunks command for the command line. Returns error only as per cobra.
func RunChunks(cmd *cobra.Command, args []string) error {
	opts := chunksFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("chunks", &opts.Globals)
	return opts.ChunksInternal()
}

// ServeChunks handles the chunks command for the API. Returns an error.
func ServeChunks(w http.ResponseWriter, r *http.Request) error {
	opts := chunksFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("chunks", w, &opts.Globals)
	err := opts.ChunksInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("chunks", err, &opts.Globals)
	return err
}

// ChunksInternal handles the internal workings of the chunks command. Returns an error.
func (opts *ChunksOptions) ChunksInternal() error {
	var err error
	if err = opts.validateChunks(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra chunks"
	// EXISTING_CODE
	chain := opts.Globals.Chain
	blockNums, err := identifiers.GetBlockNumbers(chain, opts.BlockIds)
	if err != nil {
		return err
	}
	if opts.Globals.TestMode && len(blockNums) > 200 {
		blockNums = blockNums[:200]
	}
	// EXISTING_CODE
	if opts.Check {
		err = opts.HandleCheck(blockNums)
	} else if opts.List {
		err = opts.HandleList(blockNums)
	} else if opts.Unpin {
		err = opts.HandleUnpin(blockNums)
	} else if len(opts.Tag) > 0 {
		err = opts.HandleTag(blockNums)
	} else if opts.Diff {
		err = opts.HandleDiff(blockNums)
	} else if opts.Pin {
		err = opts.HandlePin(blockNums)
	} else if opts.Publish {
		err = opts.HandlePublish(blockNums)
	} else if opts.Truncate != base.NOPOSN {
		err = opts.HandleTruncate(blockNums)
	} else {
		err = opts.HandleShow(blockNums)
	}
	timer.Report(msg)

	return err
}

// GetChunksOptions returns the options for this tool so other tools may use it.
func GetChunksOptions(args []string, g *globals.GlobalOptions) *ChunksOptions {
	ret := chunksFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}
