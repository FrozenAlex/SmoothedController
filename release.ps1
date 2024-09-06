$branch = "main";

# Get version from cli argument
$modVersion = $args[0]

# Check if the input is empty
if ($modVersion -eq "") {
    Write-Host "No version given! (example: 0.2.12)"
    exit
}

# Check if version is in format x.x.x and have no other characters 
if ($modVersion -notmatch '^\d+\.\d+\.\d+$') {
    Write-Host "Version is not in format x.x.x (example: 0.2.12) or have other characters! it's the first argument"
    exit
}


# Print version
Write-Host "Gonna do version: $modVersion"


# Create new tags array
$new_tags = @()

# Create tags
$new_tags += "v$modVersion"

# Print tags
Write-Host "New tags"
Write-Host "-----------------"
# Loop through versions and tags 1:1
for ($i = 0; $i -lt $new_tags.Length; $i++) {
    # Get tag
    $tag = $new_tags[$i]
    
    # Print tag
    Write-Host "$tag"
}
Write-Host "-----------------"

# Ask if we should continue
Write-Host "Continue? (y/n)"
$continue = Read-Host

# If not then exit
if ($continue -ne "y") {
    exit
}

# Loop through tags
for ($i = 0; $i -lt $new_tags.Length; $i++) {
    # Get tag
    $tag = $new_tags[$i]

    # Check if the tag already exists, if it does then skip it
    $tagExists = git tag | Select-String -Pattern $tag
    if ($tagExists) {
        Write-Host "Tag $tag already exists! skipping..."
        continue
    }


    
    Write-Host "Created tag: $tag from branch: $branch" 
    # Create tag
    git tag $tag $branch -m "Release $modVersion for Beat Saber"
}


# Ask if we should push
Write-Host "Push tags? (y/n)"
$push = Read-Host

# If not then exit
if ($push -ne "y") {
    exit
}


# Push tags
for ($i = 0; $i -lt $new_tags.Length; $i++) {
    # Get tag
    $tag = $new_tags[$i]

    
    Write-Host "Pushing $tag"
    # Push tag
    git push origin $tag
}

Write-Host "Done! All tags pushed! Yaay! 🎉"