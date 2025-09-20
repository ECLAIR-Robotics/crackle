#!/usr/bin/env bash
set -euo pipefail

KEY_NAME="id_ed25519"
KEY_DIR="${HOME}/.ssh"
KEY_PATH="${KEY_DIR}/${KEY_NAME}"
GITHUB_KEYS_URL="https://github.com/settings/keys"

log() { printf "\033[1;32m[setup]\033[0m %s\n" "$*"; }
warn() { printf "\033[1;33m[warn]\033[0m %s\n" "$*"; }
err() { printf "\033[1;31m[err]\033[0m  %s\n" "$*" >&2; }

open_url() {
  local url="$1"
  if command -v xdg-open >/dev/null 2>&1; then
    xdg-open "$url" >/dev/null 2>&1 || true
  elif command -v open >/dev/null 2>&1; then
    open "$url" >/dev/null 2>&1 || true
  elif command -v wslview >/dev/null 2>&1; then
    wslview "$url" >/dev/null 2>&1 || true
  elif command -v cmd.exe >/dev/null 2>&1; then
    cmd.exe /c start "$url" >/dev/null 2>&1 || true
  else
    warn "Couldn't auto-open a browser. Visit: ${url}"
  fi
}

copy_to_clipboard() {
  # Best-effort copy to clipboard on Linux/macOS/WSL
  if command -v xclip >/dev/null 2>&1; then
    xclip -selection clipboard
  elif command -v pbcopy >/dev/null 2>&1; then
    pbcopy
  elif command -v clip.exe >/dev/null 2>&1; then
    clip.exe
  else
    return 1
  fi
}

# --- Require email argument ---
if [ "$#" -ne 1 ]; then
  err "Usage: $0 <email>"
  exit 1
fi
EMAIL="$1"

# --- Check deps ---
if ! command -v ssh-keygen >/dev/null 2>&1; then
  err "ssh-keygen not found. Please install OpenSSH."
  exit 1
fi

# --- Ensure ~/.ssh directory exists ---
if [ ! -d "$KEY_DIR" ]; then
  log "Creating ${KEY_DIR}"
  mkdir -p "$KEY_DIR"
  chmod 700 "$KEY_DIR"
fi

# --- If key exists, show and proceed to wait step anyway (in case user hasn't added it) ---
if [ -f "${KEY_PATH}" ] && [ -f "${KEY_PATH}.pub" ]; then
  log "SSH key already exists at ${KEY_PATH}"
else
  # --- Generate key ---
  log "Generating a new Ed25519 SSH key at ${KEY_PATH} with email '${EMAIL}'"
  ssh-keygen -t ed25519 -C "$EMAIL" -f "$KEY_PATH" -N ""
  chmod 600 "$KEY_PATH"
  chmod 644 "${KEY_PATH}.pub"
  log "SSH key created successfully!"
fi

# --- Show public key and copy to clipboard if possible ---
log "Public key (add this to GitHub/GitLab/etc.):"
echo "------------------------------------------------------------"
cat "${KEY_PATH}.pub"
echo "------------------------------------------------------------"

if copy_to_clipboard < "${KEY_PATH}.pub"; then
  log "Copied public key to your clipboard."
else
  warn "Install xclip/pbcopy (or use WSL clip.exe) for easy copying."
fi

# --- Offer to open GitHub SSH keys page ---
if [ -t 0 ]; then
  read -rp "Open GitHub SSH Keys page now? [Y/n]: " open_ans
  open_ans="${open_ans:-Y}"
  if [[ "$open_ans" =~ ^[Yy]$ ]]; then
    open_url "$GITHUB_KEYS_URL"
  fi
fi

# --- Wait for user to finish adding the key ---
if [ -t 0 ]; then
  echo
  log "Add the key above to your GitHub account at ${GITHUB_KEYS_URL}"
  read -rp "Press ENTER after you've added the key to continue..." _
else
  warn "Non-interactive shell detected. Make sure you add the key at ${GITHUB_KEYS_URL} before proceeding."
fi

# --- Optional quick test ---
if command -v ssh >/dev/null 2>&1 && [ -t 0 ]; then
  read -rp "Test GitHub SSH access now? (runs 'ssh -T git@github.com') [y/N]: " test_ans
  if [[ "${test_ans:-N}" =~ ^[Yy]$ ]]; then
    log "Testing SSH connection to GitHub (you may see a host authenticity prompt)..."
    # Accept new host key automatically on first connect; do not add to known_hosts permanently.
    ssh -o StrictHostKeyChecking=accept-new -o UserKnownHostsFile=/dev/null -T git@github.com || true
  fi
fi

log "GitHub SSH Setup step complete."

mkdir -p ./crackle_ws/src
cd crackle_ws/src
git clone --recursive git@github.com:ECLAIR-Robotics/crackle.git
cd crackle
bash bash_scripts/env_setup.sh 
