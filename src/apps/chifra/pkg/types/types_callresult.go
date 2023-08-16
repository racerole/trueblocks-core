// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import (
	"fmt"
	"io"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
)

// EXISTING_CODE

type RawCallResult struct {
	Address          string   `json:"address"`
	BlockNumber      string   `json:"blockNumber"`
	EncodedArguments string   `json:"encodedArguments"`
	Encoding         string   `json:"encoding"`
	Name             string   `json:"name"`
	Outputs          []string `json:"outputs"`
	Signature        string   `json:"signature"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleCallResult struct {
	Address          base.Address      `json:"address"`
	BlockNumber      base.Blknum       `json:"blockNumber"`
	EncodedArguments string            `json:"encodedArguments"`
	Encoding         string            `json:"encoding"`
	Name             string            `json:"name"`
	Outputs          map[string]string `json:"outputs"`
	Signature        string            `json:"signature"`
	raw              *RawCallResult    `json:"-"`
	// EXISTING_CODE
	RawReturn string
	// EXISTING_CODE
}

func (s *SimpleCallResult) Raw() *RawCallResult {
	return s.raw
}

func (s *SimpleCallResult) SetRaw(raw *RawCallResult) {
	s.raw = raw
}

func (s *SimpleCallResult) Model(verbose bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	callResult := map[string]any{
		"name":      s.Name,
		"signature": s.Signature,
		"encoding":  s.Encoding,
		"outputs":   s.Outputs,
	}
	model = map[string]any{
		"blockNumber": s.BlockNumber,
		"address":     s.Address.Hex(),
		"encoding":    s.Encoding,
		"bytes":       s.EncodedArguments,
		"callResult":  callResult,
	}

	if format == "json" {
		return Model{
			Data: model,
		}
	}

	model["signature"] = s.Signature
	model["compressedResult"] = makeCompressed(s.Outputs)
	order = []string{
		"blockNumber",
		"address",
		"signature",
		"encoding",
		"bytes",
		"compressedResult",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// --> cacheable by address_and_block
func (s *SimpleCallResult) CacheName() string {
	return "CallResult"
}

func (s *SimpleCallResult) CacheId() string {
	return fmt.Sprintf("%s-%09d", s.Address.Hex()[2:], s.BlockNumber)
}

func (s *SimpleCallResult) CacheLocation() (directory string, extension string) {
	paddedId := s.CacheId()
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]

	subFolder := strings.ToLower(s.CacheName()) + "s"
	directory = filepath.Join(subFolder, filepath.Join(parts...))
	extension = "bin"

	return
}

func (s *SimpleCallResult) MarshalCache(writer io.Writer) (err error) {
	// Address
	if err = cache.WriteValue(writer, s.Address); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}

	// EncodedArguments
	if err = cache.WriteValue(writer, s.EncodedArguments); err != nil {
		return err
	}

	// Encoding
	if err = cache.WriteValue(writer, s.Encoding); err != nil {
		return err
	}

	// Name
	if err = cache.WriteValue(writer, s.Name); err != nil {
		return err
	}

	// Outputs
	if err = cache.WriteValue(writer, s.Outputs); err != nil {
		return err
	}

	// Signature
	if err = cache.WriteValue(writer, s.Signature); err != nil {
		return err
	}

	return nil
}

func (s *SimpleCallResult) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	// Address
	if err = cache.ReadValue(reader, &s.Address, version); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.ReadValue(reader, &s.BlockNumber, version); err != nil {
		return err
	}

	// EncodedArguments
	if err = cache.ReadValue(reader, &s.EncodedArguments, version); err != nil {
		return err
	}

	// Encoding
	if err = cache.ReadValue(reader, &s.Encoding, version); err != nil {
		return err
	}

	// Name
	if err = cache.ReadValue(reader, &s.Name, version); err != nil {
		return err
	}

	// Outputs
	if err = cache.ReadValue(reader, &s.Outputs, version); err != nil {
		return err
	}

	// Signature
	if err = cache.ReadValue(reader, &s.Signature, version); err != nil {
		return err
	}

	return nil
}

// EXISTING_CODE
// EXISTING_CODE
